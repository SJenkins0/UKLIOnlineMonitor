/* ---------------- main.cc ---------------- *
 * This will extract a variety of variables  *
 * from the diffuser root files for the UK   *
 * light injection system.                   *
 *                                           *
 * SOFTWARE REQUIRED:                        *
 * ---> ROOT                                 *
 * ---> position.h                           *
 * ---> rndFunc.h                            *
 * Adapted from Jordan's original scripts    *
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

enum class topGeom{
  kTot = 0,
  kBottom,
  kBarrel,
  kBarzy,
  kBarzylt0
};

void extract_dark_data(std::ifstream &file,  std::vector<UInt_t> &time, std::vector<float> &chg);
void extract_data(std::ifstream &file, std::vector<double> &totQ,std::vector<double> &barrelQ,std::vector<UInt_t> &dateEv,std::vector<double> &bottomQ,std::vector<double> &bz0y0Q,std::vector<double> &bzylt0Q);
void format_plots(TGraphErrors *graph1,TLegend *leg ,float min, float max, std::string timeform, std::string yaxis, std::string title);
void normalisation(std::vector<float> &in, float normVal, std::vector<float> &error);
void setMinMax(TGraphErrors *g);
int getPMTsInTop(const topGeom& geom);
float sdCalc(std::vector<float> vect);

int main(int argc, char *argv[]){

  
  int oNameSwitch = 0;
  //std::string ldir = "/disk02/calib3/usr/ukli/monitoring/plotting";
  std::string ldir = "/disk03/calib4/usr/ukli/monitoring/plotting";
  std::string endname = "";
  std::string outname = "";
  int opt;
  while ((opt = getopt(argc, argv, ":o:l:")) != -1){
    switch (opt)
      {
      case 'o':
	oNameSwitch = 1;
	outname = optarg;
	break;
      case 'l':
	ldir = optarg;
	break;
      case ':':
	printf("\033[1;31m[ERROR]\033[0m -%c requires an argument.\n",optopt);
	return 0;
      case '?':
	printf("\033[1;33m[ERROR]\033[0m -%c is an unknown argument... just ignoring it.\n",optopt);
	break;
      }
  }


  //Get the noise data
  std::ifstream dark_file("/disk03/calib4/usr/ukli/monitoring/plotting/noise.dat");
  std::vector<UInt_t> dark_time;
  std::vector<float> dark_charge;
  extract_dark_data(dark_file, dark_time, dark_charge);
  std::cout << dark_charge.size() << std::endl;
  
  std::string fileEx = ldir + "/top_dif.dat";
  std::cout << fileEx << std::endl;
  
  time_t timer;
  time(&timer);

  gStyle->SetCanvasColor(0);
  gStyle->SetTitleColor(0);
  gStyle->SetOptStat(0);  

  std::vector<UInt_t> dateEv;
  std::vector<double> totQ, barrelQ, bottomQ, bz0y0Q, bzylt0Q;
  std::ifstream file(fileEx);
  extract_data(file,totQ,barrelQ,dateEv,bottomQ,bz0y0Q,bzylt0Q);

  //Loop over the three time periods here
  //period == 0: all time
  //period == 1: 6 hours
  //period == 2: 24 hours
  int avenum, time;
  std::string periodTitle, periodFileEnd, plotScale;

  for (int period=0; period<3; period++){
    //Need to set period specific values
    if (period == 1){
      avenum = 100;
      time = timer-21600;
    }
    else if (period == 2){
      avenum = 400;
      time = timer-86400;
    }
    else avenum = 2000;
  
    std::cout << "Running for period " << period << std::endl;
    
    std::vector<float> dateVec;
    std::vector<float> totDataPoints,      totQVec,     errorTotVec;
    std::vector<float> barrelDataPoints,   barrelVec,   errorBarVec;
    std::vector<float> bottomDataPoints,   bottomVec,   errorBotVec;
    std::vector<float> bzyDataPoints,      bzyVec,      errorBzyVec;
    std::vector<float> bzylt0DataPoints,   bzylt0Vec,   errorBzylt0Vec;
    std::vector<float> bzyRatioDataPoints, bzyRatioVec, errorBzyRatioVec;
  
    float averageTot = 0;
    float averageBot = 0;
    float averageBar = 0;
    float averageBzy = 0;
    float averageBzylt0 = 0;
    float averageBzyRatio = 0;
    float errorTot,errorBar, errorBot, errorBzy, errorBzylt0, errorBzyRatio;

    //unnormalised versions
    std::vector<float> barrelDataPoints_un,   barrelVec_un,   errorBarVec_un;
    std::vector<float> bottomDataPoints_un,   bottomVec_un,   errorBotVec_un;
    std::vector<float> bzyDataPoints_un,      bzyVec_un,      errorBzyVec_un;
    std::vector<float> bzylt0DataPoints_un,   bzylt0Vec_un,   errorBzylt0Vec_un;
  
    float averageBot_un = 0;
    float averageBar_un = 0;
    float averageBzy_un = 0;
    float averageBzylt0_un = 0;
    float errorBar_un, errorBot_un, errorBzy_un, errorBzylt0_un;
  
    int count = 0;
    UInt_t lastTime = 0;
    
    //Loop over all hits in the total charge vector
    for (int i=0; i<totQ.size(); i++){
      //Skip if same time as previous (processing method occasionally causes double entries
      if(dateEv.at(i) == lastTime) continue;
      lastTime = dateEv.at(i);

      
      //Remove large or small outliers
      if (totQ.at(i) > 30000 && totQ.at(i) < 190000){

	//If restricted time period (6 or 24 hour), check the time
	//Skip if before
	if (period > 0 && dateEv.at(i) < time) continue;
      
	if (i == 0){
	  count++;
	  averageTot += totQ.at(i);
	  averageBot += bottomQ.at(i)/totQ.at(i);
	  averageBar += barrelQ.at(i)/totQ.at(i);
	  averageBzy += bz0y0Q.at(i)/totQ.at(i);
	  averageBzylt0 += bzylt0Q.at(i)/totQ.at(i);
	  averageBzyRatio += bzylt0Q.at(i)/bz0y0Q.at(i);
      
	  bottomDataPoints.push_back(bottomQ.at(i)/totQ.at(i));
	  barrelDataPoints.push_back(barrelQ.at(i)/totQ.at(i));
	  bzyDataPoints.push_back(bz0y0Q.at(i)/totQ.at(i));
	  bzylt0DataPoints.push_back(bzylt0Q.at(i)/totQ.at(i));
	  bzyRatioDataPoints.push_back(bzylt0Q.at(i)/bz0y0Q.at(i));
	  totDataPoints.push_back(totQ.at(i));

	  //Unnormalised
	  averageBot_un += bottomQ.at(i);
	  averageBar_un += barrelQ.at(i);
	  averageBzy_un += bz0y0Q.at(i);
	  averageBzylt0_un += bzylt0Q.at(i);
      
	  bottomDataPoints_un.push_back(bottomQ.at(i)/totQ.at(i));
	  barrelDataPoints_un.push_back(barrelQ.at(i)/totQ.at(i));
	  bzyDataPoints_un.push_back(bz0y0Q.at(i)/totQ.at(i));
	  bzylt0DataPoints_un.push_back(bzylt0Q.at(i)/totQ.at(i));
      
	}
	else if (i % avenum == 0){
	  count++;
	  // Average Calculation
	  averageTot += totQ.at(i);
	  averageTot /= count;

	  averageBot += bottomQ.at(i)/totQ.at(i);
	  averageBar += barrelQ.at(i)/totQ.at(i);
	  averageBzy += bz0y0Q.at(i)/totQ.at(i);
	  averageBzylt0 += bzylt0Q.at(i)/totQ.at(i);
	  averageBzyRatio += bzylt0Q.at(i)/bz0y0Q.at(i);
	  averageBot /= count;
	  averageBar /= count;
	  averageBzy /= count;
	  averageBzylt0 /= count;
	  averageBzyRatio /= count;

	  bottomDataPoints.push_back(bottomQ.at(i)/totQ.at(i));
	  barrelDataPoints.push_back(barrelQ.at(i)/totQ.at(i));
	  bzyDataPoints.push_back(bz0y0Q.at(i)/totQ.at(i));
	  bzylt0DataPoints.push_back(bzylt0Q.at(i)/totQ.at(i));
	  bzyRatioDataPoints.push_back(bzylt0Q.at(i)/bz0y0Q.at(i));
	  totDataPoints.push_back(totQ.at(i));


	  //Unnormalised
	  averageBot_un += bottomQ.at(i);
	  averageBar_un += barrelQ.at(i);
	  averageBzy_un += bz0y0Q.at(i);
	  averageBzylt0_un += bzylt0Q.at(i);
	  averageBot_un /= count;
	  averageBar_un /= count;
	  averageBzy_un /= count;
	  averageBzylt0_un /= count;

	  bottomDataPoints_un.push_back(bottomQ.at(i));
	  barrelDataPoints_un.push_back(barrelQ.at(i));
	  bzyDataPoints_un.push_back(bz0y0Q.at(i));
	  bzylt0DataPoints_un.push_back(bzylt0Q.at(i));

	  /*
	  float max = *std::max_element(totDataPoints.begin(),totDataPoints.end());
	  float min = *std::min_element(totDataPoints.begin(),totDataPoints.end());
	  errorTot = (max - min) / (2 * std::sqrt(count));
	  float maxba = *std::max_element(barrelDataPoints.begin(),barrelDataPoints.end());
	  float minba = *std::min_element(barrelDataPoints.begin(),barrelDataPoints.end());
	  errorBar = (maxba - minba) / (2 * std::sqrt(count));
	  float maxbo = *std::max_element(bottomDataPoints.begin(),bottomDataPoints.end());
	  float minbo = *std::min_element(bottomDataPoints.begin(),bottomDataPoints.end());
	  errorBot = (maxbo - minbo) / (2 * std::sqrt(count));
	  float maxbzy = *std::max_element(bzyDataPoints.begin(),bzyDataPoints.end());
	  float minbzy = *std::min_element(bzyDataPoints.begin(),bzyDataPoints.end());
	  errorBzy = (maxbzy - minbzy) / (2 * std::sqrt(count));
	  float maxbzylt0 = *std::max_element(bzylt0DataPoints.begin(),bzylt0DataPoints.end());
	  float minbzylt0 = *std::min_element(bzylt0DataPoints.begin(),bzylt0DataPoints.end());
	  errorBzylt0 = (maxbzylt0 - minbzylt0) / (2 * std::sqrt(count));
	  float maxbzyratio = *std::max_element(bzyRatioDataPoints.begin(),bzyRatioDataPoints.end());
	  float minbzyratio = *std::min_element(bzyRatioDataPoints.begin(),bzyRatioDataPoints.end());
	  errorBzyRatio = (maxbzyratio - minbzyratio) / (2 * std::sqrt(count));
	  */

	  errorTot = sdCalc(totDataPoints)/std::sqrt(count);
	  errorBar = sdCalc(barrelDataPoints)/std::sqrt(count);
	  errorBot = sdCalc(bottomDataPoints)/std::sqrt(count);
	  errorBzy = sdCalc(bzyDataPoints)/std::sqrt(count);
	  errorBzylt0 = sdCalc(bzylt0DataPoints)/std::sqrt(count);
	  errorBzyRatio = sdCalc(bzyRatioDataPoints)/std::sqrt(count);

	  errorTotVec.push_back(errorTot);
	  errorBotVec.push_back(errorBot);
	  errorBarVec.push_back(errorBar);
	  errorBzyVec.push_back(errorBzy);
	  errorBzylt0Vec.push_back(errorBzylt0);
	  errorBzyRatioVec.push_back(errorBzyRatio);

	  //unnormalised
	  /*
	  maxba = *std::max_element(barrelDataPoints_un.begin(),barrelDataPoints_un.end());
	  minba = *std::min_element(barrelDataPoints_un.begin(),barrelDataPoints_un.end());
	  errorBar = (maxba - minba) / (2 * std::sqrt(count));
	  maxbo = *std::max_element(bottomDataPoints_un.begin(),bottomDataPoints_un.end());
	  minbo = *std::min_element(bottomDataPoints_un.begin(),bottomDataPoints_un.end());
	  errorBot = (maxbo - minbo) / (2 * std::sqrt(count));
	  maxbzy = *std::max_element(bzyDataPoints_un.begin(),bzyDataPoints_un.end());
	  minbzy = *std::min_element(bzyDataPoints_un.begin(),bzyDataPoints_un.end());
	  errorBzy = (maxbzy - minbzy) / (2 * std::sqrt(count));
	  maxbzylt0 = *std::max_element(bzylt0DataPoints_un.begin(),bzylt0DataPoints_un.end());
	  minbzylt0 = *std::min_element(bzylt0DataPoints_un.begin(),bzylt0DataPoints_un.end());
	  errorBzylt0 = (maxbzylt0 - minbzylt0) / (2 * std::sqrt(count));
	  */
	  
	  errorBar = sdCalc(barrelDataPoints_un)/std::sqrt(count);
	  errorBot = sdCalc(bottomDataPoints_un)/std::sqrt(count);
	  errorBzy = sdCalc(bzyDataPoints_un)/std::sqrt(count);
	  errorBzylt0 = sdCalc(bzylt0DataPoints_un)/std::sqrt(count);

	  errorBotVec_un.push_back(errorBot);
	  errorBarVec_un.push_back(errorBar);
	  errorBzyVec_un.push_back(errorBzy);
	  errorBzylt0Vec_un.push_back(errorBzylt0);

      
	  totDataPoints.clear();
	  bottomDataPoints.clear();
	  barrelDataPoints.clear();
	  bzyDataPoints.clear();
	  bzylt0DataPoints.clear();
	  bzyRatioDataPoints.clear();

	  bottomDataPoints_un.clear();
	  barrelDataPoints_un.clear();
	  bzyDataPoints_un.clear();
	  bzylt0DataPoints_un.clear();
      
      
	  // Sort out average vectors
	  dateVec.push_back(dateEv.at(i));
	  totQVec.push_back(averageTot);
	  bottomVec.push_back(averageBot);
	  barrelVec.push_back(averageBar);
	  bzyVec.push_back(averageBzy);
	  bzylt0Vec.push_back(averageBzylt0);
	  bzyRatioVec.push_back(averageBzyRatio);
	  bottomVec_un.push_back(averageBot_un);
	  barrelVec_un.push_back(averageBar_un);
	  bzyVec_un.push_back(averageBzy_un);
	  bzylt0Vec_un.push_back(averageBzylt0_un);
	  averageTot = 0;
	  averageBot = 0;
	  averageBar = 0;
	  averageBzy = 0;
	  averageBzylt0 = 0;
	  averageBzyRatio = 0;
	  averageBot_un = 0;
	  averageBar_un = 0;
	  averageBzy_un = 0;
	  averageBzylt0_un = 0;
	  count = 0;
	}
	else {

	  count++;
	  averageTot += totQ.at(i);
	  averageBot += bottomQ.at(i)/totQ.at(i);
	  averageBar += barrelQ.at(i)/totQ.at(i);
	  averageBzy += bz0y0Q.at(i)/totQ.at(i);
	  averageBzylt0 += bzylt0Q.at(i)/totQ.at(i);
	  averageBzyRatio += bzylt0Q.at(i)/bz0y0Q.at(i);
      
	  bottomDataPoints.push_back(bottomQ.at(i)/totQ.at(i));
	  barrelDataPoints.push_back(barrelQ.at(i)/totQ.at(i));
	  bzyDataPoints.push_back(bz0y0Q.at(i)/totQ.at(i));
	  bzylt0DataPoints.push_back(bzylt0Q.at(i)/totQ.at(i));
	  bzyRatioDataPoints.push_back(bzylt0Q.at(i)/bz0y0Q.at(i));
	  totDataPoints.push_back(totQ.at(i));

	  //Unnormalised
	  averageBot_un += bottomQ.at(i);
	  averageBar_un += barrelQ.at(i);
	  averageBzy_un += bz0y0Q.at(i);
	  averageBzylt0_un += bzylt0Q.at(i);
      
	  bottomDataPoints_un.push_back(bottomQ.at(i)/totQ.at(i));
	  barrelDataPoints_un.push_back(barrelQ.at(i)/totQ.at(i));
	  bzyDataPoints_un.push_back(bz0y0Q.at(i)/totQ.at(i));
	  bzylt0DataPoints_un.push_back(bzylt0Q.at(i)/totQ.at(i));
	
	}
      }
    }//end of loop over data in totQ vector

    //Only continue if there are actually events to subtract from - otherwise this will segfault
    if(totQVec.size() > 1){


      //Unnormalised and SKQ normalised values are constructed
      //Now we do dark noise subtraction
      //Start by averaging over the dark noise data for the relevant time period
      std::vector<float> darkChargeData, darkQVec, darkQErrVec;
      float chgSum = 0;
      float darkQErr;
      int darkCount = 0;
      long long int injTimeLow, injTimeHigh;
      const int noiseT = 500; //500 ns

      std::vector<float> totQNSVec, barrelQNSVec, bottomQNSVec, bzyQNSVec, bzylt0QNSVec, bzyRatioQNSVec, dateVecNS;
      std::vector<float> errTotQNSVec, errBarrelQNSVec, errBottomQNSVec, errBzyQNSVec, errBzylt0QNSVec, errBzyRatioQNSVec;
      double totQNS, barrelQNS, bottomQNS, bzyQNS, bzylt0QNS, bzyRatioQNS;
      double errTotQNS, errBarrelQNS, errBottomQNS, errBzyQNS, errBzylt0QNS, errBzyRatioQNS;
      int nPMTs = 11129 - 90; //Could do with this being automatically updated, but fine for now
      int nCutPMTs;
      double cutScaleTot, cutScaleBarrel, cutScaleBottom, cutScaleBzy, cutScaleBzylt0;
      const double topWindow = 1000.;

    
      //Get the number of PMTs in the different geometric cuts, use them to get scale factors
      //Total - sanity check, should be all PMTs
      nCutPMTs = getPMTsInTop(topGeom::kTot);
      cutScaleTot = (topWindow/(double)noiseT) * (nCutPMTs/(double)nPMTs);
      std::cout << "Top tot scaling = " << cutScaleTot << std::endl;
      //Barrel
      nCutPMTs = getPMTsInTop(topGeom::kBarrel);
      cutScaleBarrel = (topWindow/(double)noiseT) * (nCutPMTs/(double)nPMTs);
      std::cout << "Top barrel scaling = " << cutScaleBarrel << std::endl;
      //Bottom
      nCutPMTs = getPMTsInTop(topGeom::kBottom);
      cutScaleBottom = (topWindow/(double)noiseT) * (nCutPMTs/(double)nPMTs);
      std::cout << "Top bottom scaling = " << cutScaleBottom << std::endl;
      //Barrel zy gt 0
      nCutPMTs = getPMTsInTop(topGeom::kBarzy);
      cutScaleBzy = (topWindow/(double)noiseT) * (nCutPMTs/(double)nPMTs);
      std::cout << "Top barrel zygt0 scaling = " << cutScaleBzy << std::endl;
      //Barrel zy lt 0
      nCutPMTs = getPMTsInTop(topGeom::kBarzylt0);
      cutScaleBzylt0 = (topWindow/(double)noiseT) * (nCutPMTs/(double)nPMTs);
      std::cout << "Top barrel zylt0 scaling = " << cutScaleBzylt0 << std::endl;

      int lasti = 0;
      
      int k=0; //indexes noise vector
      for(int i=0; i<dateVec.size();){
	//Break if we've reached the end of the noise file
	if(k==dark_time.size()) break;
	//Set time limits
	if(i==0){
	  injTimeLow = dark_time.at(0);
	  if(dateVec.at(i) < injTimeLow) injTimeLow = dateVec.at(i);
	}
	else injTimeLow = dateVec.at(i-1);
	injTimeHigh = dateVec.at(i);
	
	if(dark_time.at(k) >= injTimeHigh){
	  i++;
	  continue;
	}

	//Reset
	bool skip = true; //Starts as true, will get set to false once we reach relevant data
	while(dark_time.at(k) >= injTimeLow && dark_time.at(k) < injTimeHigh){

	  if(skip) skip = false;

	  if(dark_charge.at(k)<1000){//remove large outliers
	    chgSum += dark_charge.at(k);
	    darkChargeData.push_back(dark_charge.at(k));
	    darkCount++;
	  }
	  k++;
	  //break out if we've reached the end
	  if(k==dark_time.size()){std::cout << "Breaking at k = " << k << std::endl; break;}
	}
	//True if the while loop got skipped, in which case k needs incrementing
	if(skip) k++;
	else{

	  lasti=i;
	  chgSum /= darkCount;
	  darkQVec.push_back(chgSum);

	  dateVecNS.push_back(dateVec.at(i));
	  /*
	  double max = *std::max_element(darkChargeData.begin(), darkChargeData.end());
	  double min = *std::min_element(darkChargeData.begin(), darkChargeData.end());
	  darkQErr = (max - min) / (2 * std::sqrt(darkCount));
	  */
	  darkQErr = sdCalc(darkChargeData)/std::sqrt(darkCount);
	  darkQErrVec.push_back(darkQErr);

	  //Do the noise subtraction from unnormalised vectors
	  totQNS = totQVec.at(i) - cutScaleTot*chgSum;
	  errTotQNS = std::sqrt( pow(errorTotVec.at(i), 2) + pow(cutScaleTot*darkQErr, 2));
	  totQNSVec.push_back(totQNS);
	  errTotQNSVec.push_back(errTotQNS);

	  //Barrel
	  barrelQNS = barrelVec_un.at(i) - cutScaleBarrel*chgSum;
	  errBarrelQNS = std::sqrt( pow(errorBarVec_un.at(i), 2) + pow(cutScaleBarrel*darkQErr, 2));
	  barrelQNSVec.push_back(barrelQNS);
	  errBarrelQNSVec.push_back(errBarrelQNS);

	  //Bottom
	  bottomQNS = bottomVec_un.at(i) - cutScaleBottom*chgSum;
	  errBottomQNS = std::sqrt( pow(errorBotVec_un.at(i), 2) + pow(cutScaleBottom*darkQErr, 2));
	  bottomQNSVec.push_back(bottomQNS);
	  errBottomQNSVec.push_back(errBottomQNS);

	  //Bzy
	  bzyQNS = bzyVec_un.at(i) - cutScaleBzy*chgSum;
	  errBzyQNS = std::sqrt( pow(errorBzyVec_un.at(i), 2) + pow(cutScaleBzy*darkQErr, 2));
	  bzyQNSVec.push_back(bzyQNS);
	  errBzyQNSVec.push_back(errBzyQNS);

	  //Bzylt0
	  bzylt0QNS = bzylt0Vec_un.at(i) - cutScaleBzylt0*chgSum;
	  errBzylt0QNS = std::sqrt( pow(errorBzylt0Vec_un.at(i), 2) + pow(cutScaleBzylt0*darkQErr, 2));
	  bzylt0QNSVec.push_back(bzylt0QNS);
	  errBzylt0QNSVec.push_back(errBzylt0QNS);

	  //Ratio - longest/shortest
	  bzyRatioQNS = bzylt0QNS/bzyQNS;
	  errBzyRatioQNS = bzyRatioQNS * std::sqrt( pow(errBzylt0QNS/bzylt0QNS, 2) + pow(errBzyQNS/bzyQNS, 2));
	  bzyRatioQNSVec.push_back(bzyRatioQNS);
	  errBzyRatioQNSVec.push_back(errBzyRatioQNS);

	  chgSum = 0;
	  darkCount = 0;
	  darkChargeData.clear();
	  i++; //Only go to next data point once we reach the end of the time period
	}
      }//end of loop over date vec
   
      //Sanity check
      std::cout << "Chg vec length for tot = " << totQVec.size() << std::endl;
      std::cout << "Dark chg vec length for top = " << darkQVec.size() << std::endl;
    

      //Perform normalisations
      //only do these for shorter time periods if there is data
      //totQ currently doesn't get normalised
      if(totQVec.size() > 1){
	normalisation(bottomVec,bottomVec.at(1),errorBotVec);
	normalisation(barrelVec,barrelVec.at(1),errorBarVec);
	normalisation(bzyVec,bzyVec.at(1),errorBzyVec);
	normalisation(bzylt0Vec,bzylt0Vec.at(1),errorBzylt0Vec);
	normalisation(bzyRatioVec,bzyRatioVec.at(1),errorBzyRatioVec);

	normalisation(bottomQNSVec, bottomQNSVec.at(1), errBottomQNSVec);
	normalisation(barrelQNSVec, barrelQNSVec.at(1), errBarrelQNSVec);
	normalisation(bzyQNSVec,    bzyQNSVec.at(1),    errBzyQNSVec   );
	normalisation(bzylt0QNSVec, bzylt0QNSVec.at(1), errBzylt0QNSVec);
	normalisation(bzyRatioQNSVec, bzyRatioQNSVec.at(1), errBzyRatioQNSVec);
      }
  
   
      if (oNameSwitch == 0) outname = "/home/calib/uk_inj/monitoring/plotting/top_diffuser";
      std::string tempNameTot = "Total Q (top diffuser) ";
      std::string tempNameBot = "Normalised Bottom Q ";
      std::string tempNameBar = "Normalised Barrel Q ";
      std::string tempNameBzy = "Normalised Barrel Q (z&y > 0) ";
      std::string tempNameBzylt0 = "Normalised Barrel Q (z&y < 0) ";
      std::string tempNameBzyRatio = "Normalised charge ratio (z&y<0 / z&y>0) ";
      std::string tempNameTotNS = "Total inj Q (top diffuser) ";
      std::string tempNameBotNS = "Normalised Bottom inj Q ";
      std::string tempNameBarNS = "Normalised Barrel inj Q ";
      std::string tempNameBzyNS = "Normalised Barrel inj Q (z&y > 0) ";
      std::string tempNameBzylt0NS = "Normalised Barrel inj Q (z&y < 0) ";
      std::string tempNameBzyRatioNS = "Normalised charge inj ratio (z&y<0 / z&y>0) ";

      if(period==1){
	periodTitle = "Past 6h";
	periodFileEnd = "_six";
	plotScale = "%H:%M";
      }
      else if(period==2){
	periodTitle = "Past 24h";
	periodFileEnd = "_day";
	plotScale = "%H:%M";
      }
      else{
	periodTitle = "All time";
	periodFileEnd = "";
	plotScale = "%m-%d";
      } 

      //Total charge
      TCanvas c1("c1","", 850, 500);
      c1.SetGrid();
      TLegend *leg1 = new TLegend(0.13,0.85,0.9,0.9);
      TGraphErrors *totQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &totQVec[0], 0, &errorTotVec[0]);
      format_plots(totQ1, leg1, 140000,160000, plotScale,"Tot Q (p.e)",tempNameTot+periodTitle);
      std::string printname1 = outname + "_totQ" + periodFileEnd + ".png";
      c1.Print(printname1.c_str());

      //Bottom charge - normalised
      TCanvas c2("c2","", 850, 500);
      c2.SetGrid();
      TLegend *leg2 = new TLegend(0.75,0.8,0.9,0.9);
      TGraphErrors *botQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bottomVec[0], 0, &errorBotVec[0]);
      format_plots(botQ1, leg2, 1.37, 1.55,plotScale,"bottomQ/totQ",tempNameBot+periodTitle);
      std::string printname2 = outname + "_botQ" + periodFileEnd + ".png";
      c2.Print(printname2.c_str());

      //Barrel charge - normalised
      TCanvas c3("c3","", 850, 500);
      c3.SetGrid();
      TLegend *leg3 = new TLegend(0.75,0.8,0.9,0.9);
      TGraphErrors *barQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &barrelVec[0], 0, &errorBarVec[0]);
      format_plots(barQ1, leg3, 1.37, 1.55,plotScale,"barrelQ/totQ",tempNameBar+periodTitle);
      std::string printname3 = outname + "_barQ" + periodFileEnd + ".png";
      c3.Print(printname3.c_str());

      //Barrel z&y > 0 - normalised
      TCanvas c4("c4","", 850, 500);
      c4.SetGrid();
      TLegend *leg4 = new TLegend(0.75,0.8,0.9,0.9);
      TGraphErrors *bzyQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bzyVec[0], 0, &errorBzyVec[0]);
      format_plots(bzyQ1, leg4, 1.37, 1.55,plotScale,"bz0y0Q/totQ",tempNameBzy+periodTitle);
      std::string printname4 = outname + "_bzyQ" + periodFileEnd + ".png";
      c4.Print(printname4.c_str());

      //Barrel z&y < 0 - normalised
      TCanvas c5("c5", "", 850, 500);
      c5.SetGrid();
      TLegend *leg5 = new TLegend(0.75, 0.8, 0.9, 0.9);
      TGraphErrors *bzylt0Q1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bzylt0Vec[0], 0, &errorBzylt0Vec[0]);
      format_plots(bzylt0Q1, leg5, 0.8, 1.2, plotScale, "bzylt0Q/totQ", tempNameBzylt0+periodTitle);
      std::string printname5 = outname + "_bzylt0Q" + periodFileEnd + ".png";
      c5.Print(printname5.c_str());

      //Barrel z&y<0 / z&y>0 (longest/shortest) - normalised
      TCanvas c6("c6", "", 850, 500);
      c6.SetGrid();
      TLegend *leg6 = new TLegend(0.75, 0.8, 0.9, 0.9);
      TGraphErrors *bzyRatio = new TGraphErrors(dateVec.size(), &dateVec[0], &bzyRatioVec[0], 0, &errorBzyRatioVec[0]);
      format_plots(bzyRatio, leg6, 0.8, 1.2, plotScale, "longest/shortest", tempNameBzyRatio+periodTitle);
      std::string printname6 = outname + "_bzyRatioQ" + periodFileEnd + ".png";
      c6.Print(printname6.c_str());

    
      //Charge subtracted plots
      //Total charge
      TCanvas c11("c11","", 850, 500);
      c11.SetGrid();
      TLegend *leg11 = new TLegend(0.13,0.85,0.9,0.9);
      TGraphErrors *totQNS1 = new TGraphErrors(dateVecNS.size(), &dateVecNS[0], &totQNSVec[0], 0, &errTotQNSVec[0]);
      format_plots(totQNS1, leg11, 140000,160000, plotScale,"Tot Q (p.e)",tempNameTotNS+periodTitle);
      std::string printname11 = outname + "_totQNS" + periodFileEnd + ".png";
      c11.Print(printname11.c_str());

      //Bottom charge - normalised
      TCanvas c12("c12","", 850, 500);
      c12.SetGrid();
      TLegend *leg12 = new TLegend(0.75,0.8,0.9,0.9);
      TGraphErrors *botQNS1 = new TGraphErrors(dateVecNS.size(), &dateVecNS[0], &bottomQNSVec[0], 0, &errBottomQNSVec[0]);
      format_plots(botQNS1, leg12, 1.37, 1.55,plotScale,"bottomQ/totQ",tempNameBotNS+periodTitle);
      std::string printname12 = outname + "_botQNS" + periodFileEnd + ".png";
      c12.Print(printname12.c_str());

      //Barrel charge - normalised
      TCanvas c13("c13","", 850, 500);
      c13.SetGrid();
      TLegend *leg13 = new TLegend(0.75,0.8,0.9,0.9);
      TGraphErrors *barQNS1 = new TGraphErrors(dateVecNS.size(), &dateVecNS[0], &barrelQNSVec[0], 0, &errBarrelQNSVec[0]);
      format_plots(barQNS1, leg13, 1.37, 1.55,plotScale,"barrelQ/totQ",tempNameBarNS+periodTitle);
      std::string printname13 = outname + "_barQNS" + periodFileEnd + ".png";
      c13.Print(printname13.c_str());

      //Barrel z&y > 0 - normalised
      TCanvas c14("c14","", 850, 500);
      c14.SetGrid();
      TLegend *leg14 = new TLegend(0.75,0.8,0.9,0.9);
      TGraphErrors *bzyQNS1 = new TGraphErrors(dateVecNS.size(), &dateVecNS[0], &bzyQNSVec[0], 0, &errBzyQNSVec[0]);
      format_plots(bzyQNS1, leg14, 1.37, 1.55,plotScale,"bz0y0Q/totQ",tempNameBzyNS+periodTitle);
      std::string printname14 = outname + "_bzyQNS" + periodFileEnd + ".png";
      c14.Print(printname14.c_str());

      //Barrel z&y < 0 - normalised
      TCanvas c15("c15", "", 850, 500);
      c15.SetGrid();
      TLegend *leg15 = new TLegend(0.75, 0.8, 0.9, 0.9);
      TGraphErrors *bzylt0QNS1 = new TGraphErrors(dateVecNS.size(), &dateVecNS[0], &bzylt0QNSVec[0], 0, &errBzylt0QNSVec[0]);
      format_plots(bzylt0QNS1, leg15, 0.8, 1.2, plotScale, "bzylt0Q/totQ", tempNameBzylt0NS+periodTitle);
      std::string printname15 = outname + "_bzylt0QNS" + periodFileEnd + ".png";
      c15.Print(printname15.c_str());

      //Barrel z&y<0 / z&y>0 (longest/shortest) - normalised
      TCanvas c16("c16", "", 850, 500);
      c16.SetGrid();
      TLegend *leg16 = new TLegend(0.75, 0.8, 0.9, 0.9);
      TGraphErrors *bzyRatioNS = new TGraphErrors(dateVecNS.size(), &dateVecNS[0], &bzyRatioQNSVec[0], 0, &errBzyRatioQNSVec[0]);
      format_plots(bzyRatioNS, leg16, 0.8, 1.2, plotScale, "longest/shortest", tempNameBzyRatioNS+periodTitle);
      std::string printname16 = outname + "_bzyRatioQNS" + periodFileEnd + ".png";
      c16.Print(printname16.c_str());
    }//end of check for events
  }//end of loop over different periods
  
}

//Use geometry enums to return the number of PMTs in slices of SK
int getPMTsInTop(const topGeom& geom){

  int nPMT = -999;

  switch(geom){
  case topGeom::kTot:
    nPMT = 11039;
    break;
  case topGeom::kBottom:
    nPMT = 1748;
    break;
  case topGeom::kBarrel:
    nPMT = 4354;
    break;
  case topGeom::kBarzy:
    nPMT = 668;
    break;
  case topGeom::kBarzylt0:
    nPMT = 1686;
    break;
  default:
    std::cout << "Invalid top diffuser geometry" << std::endl;
  }
  return nPMT;
}


void extract_data(std::ifstream &file,std::vector<double> &totQ, std::vector<double> &barrelQ,std::vector<UInt_t> &dateEv,std::vector<double> &bottomQ,std::vector<double> &bz0y0Q,std::vector<double> &bzylt0Q){

  //Need to do this a convoluted way in order to sort the entries according to time
  std::vector<std::tuple<UInt_t, double, double, double, double, double> > event; //unix time, totQ, barQ, botQ, bz0y0, bzylt0
  //Get the number of lines first
  //Requires two loops over the input file, but should be faster than reallocating memory on every push_back
  std::string line;
  int nLines = 0;
  while(getline(file, line)) nLines++;

  event.reserve(nLines-1);
  totQ.reserve(nLines-1);
  barrelQ.reserve(nLines-1);
  dateEv.reserve(nLines-1);
  bottomQ.reserve(nLines-1);
  bz0y0Q.reserve(nLines-1);
  bzylt0Q.reserve(nLines-1);

  //Reset the stream
  file.clear();
  file.seekg(0, std::ios::beg);
  
  int year, month, day, hour, minute, second;
  getline(file,line);
  
  while(getline(file, line)){

    std::stringstream line_stream(line);
    std::string entry;
    std::vector<std::string> entries;
    entries.reserve(13);
    char delim = ' ';
    while(getline(line_stream, entry, delim)){
      entries.push_back(entry);
    }
    year = std::stoi(entries.at(2));
    month = std::stoi(entries.at(3));
    day = std::stoi(entries.at(4));
    hour = std::stoi(entries.at(5));
    minute = std::stoi(entries.at(6));
    second = std::stoi(entries.at(7));
    TDatime date(year+1900, month, day, hour, minute, second);

    event.push_back(std::make_tuple(date.Convert(), std::stod(entries.at(8)), std::stod(entries.at(9)), std::stod(entries.at(11)), std::stod(entries.at(10)), std::stod(entries.at(12))));
  }

  event.shrink_to_fit();
  //Now sort according to time
  std::sort(event.begin(), event.end());

  //Move into vectors
  for(int i=0; i<event.size(); i++){
    dateEv.push_back (std::get<0>(event[i]));
    totQ.push_back   (std::get<1>(event[i]));
    barrelQ.push_back(std::get<2>(event[i]));
    bottomQ.push_back(std::get<3>(event[i]));
    bz0y0Q.push_back (std::get<4>(event[i]));
    bzylt0Q.push_back(std::get<5>(event[i]));
  }
  
}


void format_plots(TGraphErrors *graph1,TLegend *leg,float min, float max, std::string timeform, std::string yaxis, std::string title){

  TDatime now;

  title = title + "(" + now.GetDate() + ")";
  
  graph1->SetMarkerStyle(20);
  graph1->SetMarkerColor(2);
  graph1->SetLineColor(2);
  graph1->SetMarkerSize(0.4);
  graph1->Draw("aZpsame");
  graph1->SetFillColor(0);
  graph1->GetYaxis()->SetTitle(yaxis.c_str());
  graph1->SetTitle(title.c_str());
  //  graph1->GetYaxis()->SetRangeUser(min,max);
  setMinMax(graph1);
  graph1->GetXaxis()->SetTimeDisplay(1);
  graph1->GetXaxis()->SetTimeFormat(timeform.c_str());
  graph1->GetXaxis()->SetTimeOffset(0,"jst");

  leg->SetBorderSize(0);
  leg->SetNColumns(5);
  leg->SetFillColor(0);
  leg->AddEntry(graph1, "B1", "p" );
  //leg->Draw();
  
}



void normalisation(std::vector<float> &in, float normVal, std::vector<float> &error){

  float prevError;
  for (int i =0; i < in.size(); i++){
    prevError = in[i];
    in[i] = in[i]/normVal;
    error[i] = error[i]*in[i]/prevError;
  }

}

//Automatically set min and max values
void setMinMax(TGraphErrors *g){

  double min, max, minerr, maxerr;
  const long long N = g->GetN();
  long long ind[N];
  //Sort smallest to largest
  TMath::Sort(N, g->GetY(), ind, false);

  min = g->GetY()[ind[0]];
  max = g->GetY()[ind[N-1]];
  minerr = g->GetErrorY(ind[0]);
  maxerr = g->GetErrorY(ind[N-1]);
  if(max > 1.003*g->GetY()[ind[N-2]]){ max = g->GetY()[ind[N-2]]; maxerr = g->GetErrorY(ind[N-2]);}
  if(min < 0.998*g->GetY()[ind[1]]){   min = g->GetY()[ind[1]]; minerr = g->GetErrorY(ind[1]);}
  
  g->GetYaxis()->SetRangeUser(0.999*(min-minerr), 1.001*(max+maxerr));

}

//function to read the dark noise .dat file and put the information into vectors passed
void extract_dark_data(std::ifstream &file, std::vector<UInt_t> &time, std::vector<float> &chg){

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

    time.push_back((UInt_t)std::stoi(entries.at(0)));
    chg.push_back(std::stod(entries.at(1)));
  }


}


float sdCalc(std::vector<float> vect){

  float mean = std::accumulate(vect.begin(),vect.end(),0.0)/vect.size();

  float sum = 0;
  float diff;
  for (int i=0;i<vect.size();i++){
    diff = pow((vect.at(i) - mean),2);
    sum += diff;
  }

  float sd = sqrt(sum/vect.size());

  return sd;
}
