/* ---------------- main.cc ---------------- *
 * This will extract a variety of variables  * 
 * from the diffuser root files for the UK   *
 * light injection system.                   *
 *                                           *
 * SOFTWARE REQUIRED:                        *
 * ---> ROOT                                 *
 * ---> position.h                           *
 * ---> rndFunc.h                            *
 *                                           *
 *                jmmcelwee1@sheffield.ac.uk *
 * ------------------------- J. McElwee ---- */

#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>
#include <fstream>
#include <numeric>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <typeinfo>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TVector3.h"

#include "position.h"
#include "time.h"
#include "rndFunc.h"

int main(int argc, char *argv[]){


  // ORGANISING ARGUMENT FLAGS
  // Set default values 
  std::string rFile = "";
  std::string injPos = "B1";
  std::string beamType = "diffuser";
  std::string nrun = "";
  float opAng = 50.0;
  int timeLowCut = 1150;
  int timeHighCut = 1450;
  int fnameSwitch = 0;
  std::string filename = "";

  bool isCol = false;
  
  // Search command options for flags
  int opt;
  while ((opt = getopt(argc, argv, ":dchi:f:o:r:")) != -1){
    switch (opt)
      {
      case 'h':
	help();
	return 0;
      case'i':
	injPos = optarg;
	break;
      case 'd':
	opAng = 50.0;
	isCol = false;
	beamType = "diffuser";
	break;
      case 'c':
	opAng = 5.0;
	isCol = true;
	beamType = "collimator";
	break;
      case 'f':
	rFile = optarg;
	break;
      case 'o':
	fnameSwitch = 1;
	filename = optarg;
	break;
      case 'r':
	nrun = optarg;
	break;
      case ':':
	printf("\033[1;31m[ERROR]\033[0m -%c requires an argument.\n",optopt);
	return 0;
      case '?':
	printf("\033[1;33m[ERROR]\033[0m -%c is an unknown argument... just ignoring it.\n",optopt);
	break;
      }
  }


  // PERFORM FILE AND VARIABLE CHECKS
  // Check for correct injector position
  TVector3 inj(1490.73, 768.14, injector(injPos));
  TVector3 tar = target(injPos);

  TVector3 tarVec = tar - inj;
  
  // Get high and low time cut
  timeLowCut = timeCut(injPos, isCol, true);
  timeHighCut = timeCut(injPos, isCol, false);
  int tofCutVal = tofCut(injPos, isCol);

  
  // Check for input file and that it exists 
  if (rFile == ""){
    std::cout << "\033[1;31m[ERROR]\033[0m What am I supposed to do without a file?" << std::endl;
    return 0;
  }
  std::ifstream fIsAlive(rFile);
  if (!fIsAlive) {
    std::cout << "\033[1;31m[ERROR]\033[0m " << rFile 
	      << " does not exist. Please supply real file." << std::endl;
    return 0;
  }

  //std::cout << "\033[1;34m[INFO]\033[0m Using injector position: " << injPos << std::endl;
  //std::cout << "\033[1;34m[INFO]\033[0m Analysing beam type: " << beamType << std::endl;
  //std::cout << "\033[1;34m[INFO]\033[0m Reading in ROOT file: " << std::endl;


  // READ INPUT FILE AND SETUP VARIABLES
  TFile infile(rFile.c_str(),"READ");
  TTree *intree = 0;
  infile.GetObject("tqtree", intree);
  infile.ls();
  //std::cout << "\033[1;34m[INFO]\033[0m Example event:" << std::endl;
  //intree->Show(0);

  Int_t year = 0;
  Int_t month = 0;
  Int_t day = 0;
  Int_t hour = 0;
  Int_t minute = 0;
  Int_t second = 0;
  Int_t run = 0;
  Int_t subrun = 0;
  Int_t nev = 0;
  std::vector<int> *ihit_vec = 0;
  std::vector<int> *cable_vec = 0;
  std::vector<float> *charge_vec = 0;
  std::vector<double> *time_vec = 0;
  std::vector<double> *pmtx_vec = 0;
  std::vector<double> *pmty_vec = 0;
  std::vector<double> *pmtz_vec = 0;
  std::vector<int> *mon_ihit_vec = 0;
  std::vector<int> *mon_cable_vec = 0;
  std::vector<float> *mon_charge_vec = 0;
  std::vector<double> *mon_time_vec = 0;
  
  intree->SetBranchAddress("year", &year);
  intree->SetBranchAddress("month", &month);
  intree->SetBranchAddress("day", &day);
  intree->SetBranchAddress("hour", &hour);
  intree->SetBranchAddress("minute", &minute);
  intree->SetBranchAddress("second", &second);
  intree->SetBranchAddress("run", &run);
  intree->SetBranchAddress("subrun", &subrun);
  intree->SetBranchAddress("nev", &nev);
  intree->SetBranchAddress("ihit_vec", &ihit_vec);
  intree->SetBranchAddress("cable_vec", &cable_vec);
  intree->SetBranchAddress("charge_vec", &charge_vec);
  intree->SetBranchAddress("time_vec", &time_vec);
  intree->SetBranchAddress("pmtx_vec", &pmtx_vec);
  intree->SetBranchAddress("pmty_vec", &pmty_vec);
  intree->SetBranchAddress("pmtz_vec", &pmtz_vec);
  intree->SetBranchAddress("mon_ihit_vec", &mon_ihit_vec);
  intree->SetBranchAddress("mon_cable_vec", &mon_cable_vec);
  intree->SetBranchAddress("mon_charge_vec", &mon_charge_vec);
  intree->SetBranchAddress("mon_time_vec", &mon_time_vec);


  // CREATE FILE FOR PUSHING DATA
  std::ofstream dataFile;
  // Check if output file name given
  if (fnameSwitch == 0) filename = beamType + "_" + injPos + "_" + nrun + ".dat";  
  dataFile.open(filename);
  std::cout << "\033[1;34m[INFO]\033[0m Creating file " << filename << std::endl;
  //dataFile << "run subrun month day hour minute second nev_tot nev_spot totQ spotQ monQ\n";

  
  // MAIN EVENT LOOP
  int nEvnt = intree->GetEntries();
  for (Int_t evnt =0; evnt < nEvnt; ++evnt){

    intree->GetEntry(evnt);
    if ((evnt + 1) % 500 == 0){
      std::cout << "\033[1;34m[INFO]\033[0m Processing event number: " << evnt + 1 << std::endl;
    }
     

    // SK CHARGE AND TIME VECTOR LOOP
    float nSpot = 0;
    float spotQ = 0;
    TVector3 pmt;

    float c = 30/1.2; // speed of light in water (cm/ns)
    
    for (int count = 0; count < ihit_vec->size(); ++count){
      
      // Time cut for injector events
      if (time_vec->at(count) > timeLowCut && time_vec->at(count) < timeHighCut){

	pmt.SetXYZ(pmtx_vec->at(count),pmty_vec->at(count),pmtz_vec->at(count));
	TVector3 photon = pmt - inj;
	float timeCor = time_vec->at(count) - photon.Mag()/c;

	float theta = photon.Angle(tarVec) * 180 / M_PI;
	
	if (timeCor < tofCutVal){

	  if (theta < opAng){
	    nSpot += 1;
	    spotQ += charge_vec->at(count);
	  }
	}

      }

      // Original spatial cut 
      /*pmt.SetXYZ(pmtx_vec->at(count),pmty_vec->at(count),pmtz_vec->at(count));
      TVector3 a = pmt - inj;
      TVector3 b = -inj;
      float theta = a.Angle(b) * 180 / M_PI; 
      if (theta < opAng){
      	nev_spot += 1;
	spotQ += charge_vec->at(count);
	}*/
    }

    float totQ = std::accumulate(charge_vec->begin(), charge_vec->end(), 0.0);
    float nev = ihit_vec->size();


    // MONITOR CHARGE VECTOR LOOP 
    float monQ; 
    int monHits = 0;
    // Loop to remove erroneous events from monitor vector
    for (int count=0; count < mon_cable_vec->size(); count++){
      if (mon_cable_vec->at(count) != 11256) continue;
      else{
	monQ = mon_charge_vec->at(count);
	monHits++;
      }
    }
    if (monHits > 1){
      std::cout << "\033[1;31m[ERROR]\033[0m Monitor has more than one hit." << std::endl;
    }


    // FILL DATA FILE WITH EVENT INFORMATION 
    dataFile << run << " " << subrun << " " << year << " " << month << " " << day << " "
             << hour << " " << minute << " " << second << " " << nev << " " << nSpot 
	     << " " << totQ << " " << spotQ << " " << monQ << "\n";
  
  }

  //std::cout << "\033[1;34m[INFO]\033[0m Closing " << filename << "." << std::endl;
  dataFile.close();
  
  return 0;
}
