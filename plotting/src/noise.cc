

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
 *                                           *
 *               s.j.jenkins@liverpool.ac.uk *
 * ------------------------- S. Jenkins ---- */

#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
#include <numeric>
#include <ctime>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <typeinfo>
#include <array>
#include <time.h>
#include <algorithm>
#include <typeinfo>
#include <tuple>
#include <utility>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TVector3.h"
#include "TDatime.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TTimeStamp.h"

//Function definitions
void extract_data(std::ifstream &file, std::vector< std::pair< UInt_t, float> > &events, std::vector<std::pair<UInt_t, float> > &rates);

//function to get the running average of noise and dump it into a text file
int main(int argc, char *argv[]){

  int oNameSwitch = 0;
  std::string ldir = "/disk03/calib4/usr/ukli/monitoring/plotting";
  std::string endname = "";
  std::string outname = "";

  std::cout << "Getting latest data for dark charge to perform subtraction." << std::endl;
  
  std::vector<std::string> fileList;
  fileList.reserve(10);
  for (int inj=0; inj<5;inj++){
    int injcount = inj+1;
    std::string fileEx = ldir + "/col_B" + std::to_string(inj+1) + ".dat";
    fileList.push_back(fileEx);
    fileEx = ldir + "/dif_B" + std::to_string(inj+1) + ".dat";
    fileList.push_back(fileEx);
  }

  
  time_t timer;
  time(&timer);

  gStyle->SetCanvasColor(0);
  gStyle->SetTitleColor(0);
  gStyle->SetOptStat(0);  

  std::vector<float> noiseQVec, errorNoiseQVec, dateVec, rateVec, errorRateVec;
  std::vector<float> noiseDataPoints, rateDataPoints;
  std::vector<std::pair<UInt_t, float> > events;
  std::vector<std::pair<UInt_t, float> > rates;

  //loop over all 10 injectors, extract the noise data, push all events back into a single 
  for (int inj=0; inj<10;inj++){
    std::ifstream file(fileList[inj]);
    std::cout << "Extracting noise from " << fileList[inj] << std::endl;
    extract_data(file, events, rates); //events is a vector of pairs which we can then sort according to the time
  }

  //sort according to the time
  std::sort(events.begin(), events.end());
  std::sort(rates.begin(), rates.end());
  
  
  //Also want to write the info to a text file to use for noise subtraction on events
  std::ofstream out;
  std::string outFile = ldir+"/noise.dat";
  out.open(outFile.c_str());

  UInt_t lastTime = 0;
  for (int i=0; i<events.size(); i++){
    UInt_t timeVal = events.at(i).first;
    float noiseVal = events.at(i).second;
    
    out << timeVal << " " << noiseVal << "\n";
  }
  out.close();
  std::cout << "Noise extraction done. Data written to " << outFile.c_str() << std::endl;
    
}



void extract_data(std::ifstream &file, std::vector< std::pair< UInt_t, float> > &events, std::vector< std::pair< UInt_t, float> > &rates){

  std::string line;
  getline(file, line);

  while(getline(file, line)){
    std::stringstream line_stream(line);
    std::string entry;
    std::vector<std::string> entries;
    char delim = ' ';

    while(getline(line_stream, entry, delim)){
      entries.push_back(entry);
    }

    int year = std::stoi(entries.at(2)) + 1900;
    int month = std::stoi(entries.at(3));
    int day = std::stoi(entries.at(4));
    int hour = std::stoi(entries.at(5));
    int minute = std::stoi(entries.at(6));
    int second = std::stoi(entries.at(7));
    float count = std::stof(entries.at(13));
    float noise = std::stof(entries.at(14));

    TDatime time;
    time.Set(year, month, day, hour, minute, second);

    auto event = std::make_pair(time.Convert(), noise);
    events.push_back(event);
    auto rate = std::make_pair(time.Convert(), count);
    rates.push_back(rate);
  }
}
