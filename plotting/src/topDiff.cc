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

void extract_dark_data(std::ifstream &file,  std::vector<UInt_t> &time, std::vector<float> &chg);
void extract_data(std::ifstream &file, std::vector<double> &totQ,std::vector<double> &barrelQ,std::vector<int> &year,std::vector<int> &month,std::vector<int> &day,std::vector<int> &hour,std::vector<int> &minute,std::vector<int> &second,std::vector<double> &bottomQ,std::vector<double> &bz0y0Q,std::vector<double> &bzylt0Q);
void format_plots(TGraph *graph1,TLegend *leg ,float min, float max, std::string timeform, std::string yaxis, std::string title);
void format_plots_multi(TGraph *graph1, TGraph *graph2, TLegend *leg,float min, float max, std::string timeform, std::string yaxis, std::string title);
void normalisation(std::vector<float> &in, float normVal, std::vector<float> &error);
void setMinMax(TGraph *g);

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

  std::vector<int> run, subrun, year, month, day, hour, minute, second;
  std::vector<double> totQ, barrelQ, bottomQ, bz0y0Q, bzylt0Q;
  std::ifstream file(fileEx);
  extract_data(file,totQ,barrelQ,year,month,day,hour,minute,second,bottomQ,bz0y0Q,bzylt0Q);

  //Loop over the three time periods here
  //period == 0: all time
  //period == 1: 6 hours
  //period == 2: 24 hours
  int avenum, time;
  std::string periodTitle, periodFileEnd;

  for (int period=0; period<3; period++){
    //Need to set period specific values
    if (period == 1){
      avenum = 20;
      time = timer-21600;
    }
    else if (period == 2){
      avenum = 100;
      time = timer-86400;
    }
    else avenum = 1000;
  
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

    //Loop over all hits in the total charge vector
    for (int i=0; i<totQ.size(); i++){
      TDatime date(year.at(i)+1900,month.at(i),day.at(i),hour.at(i),minute.at(i),second.at(i));
      //Remove large or small outliers
      if (totQ.at(i) > 30000 && totQ.at(i) < 190000){

	//If restricted time period (6 or 24 hour), check the time
	//Skip if before
	if (period > 0 && date.Convert() < time) continue;
      
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

	  errorTotVec.push_back(errorTot);
	  errorBotVec.push_back(errorBot);
	  errorBarVec.push_back(errorBar);
	  errorBzyVec.push_back(errorBzy);
	  errorBzylt0Vec.push_back(errorBzylt0);
	  errorBzyRatioVec.push_back(errorBzyRatio);

	  //unnormalised
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
	  dateVec.push_back(date.Convert());
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

    //Perform normalisations
    //only do these for shorter time periods if there is data
    //totQ currently doesn't get normalised
    if(totQVec.size() > 1){
      normalisation(bottomVec,bottomVec.at(1),errorBotVec);
      normalisation(barrelVec,barrelVec.at(1),errorBarVec);
      normalisation(bzyVec,bzyVec.at(1),errorBzyVec);
      normalisation(bzylt0Vec,bzylt0Vec.at(1),errorBzylt0Vec);
      normalisation(bzyRatioVec,bzyRatioVec.at(1),errorBzyRatioVec);
    }
  
   
    if (oNameSwitch == 0) outname = "/home/calib/uk_inj/monitoring/plotting/top_diffuser";
    std::string tempNameTot = "Total Q (top diffuser)";
    std::string tempNameBot = "Normalised Bottom Q ";
    std::string tempNameBar = "Normalised Barrel Q ";
    std::string tempNameBzy = "Normalised Barrel Q (z&y > 0) ";
    std::string tempNameBzylt0 = "Normalised Barrel Q (z&y < 0) ";
    std::string tempNameBzyRatio = "Normalised inj charge ratio (z&y<0 / z&y>0) ";

    if(period==1){
      periodTitle = "Past 6h";
      periodFileEnd = "_six";
    }
    else if(period==2){
      periodTitle = "Past 24h";
      periodFileEnd = "_day";
    }
    else{
      periodTitle = "All time";
      periodFileEnd = "";
    } 

    //Total charge
    TCanvas c1("c1","", 850, 500);
    c1.SetGrid();
    TLegend *leg1 = new TLegend(0.13,0.85,0.9,0.9);
    TGraphErrors *totQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &totQVec[0], 0, &errorTotVec[0]);
    format_plots(totQ1, leg1, 140000,160000, "%m-%d","Tot Q (p.e)",tempNameTot+periodTitle);
    std::string printname1 = outname + "_totQ" + periodFileEnd + ".png";
    c1.Print(printname1.c_str());

    //Bottom charge - normalised
    TCanvas c2("c2","", 850, 500);
    c2.SetGrid();
    TLegend *leg2 = new TLegend(0.75,0.8,0.9,0.9);
    TGraphErrors *botQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bottomVec[0], 0, &errorBotVec[0]);
    format_plots(botQ1, leg2, 1.37, 1.55,"%m-%d","bottomQ/totQ",tempNameBot+periodTitle);
    std::string printname2 = outname + "_botQ" + periodFileEnd + ".png";
    c2.Print(printname2.c_str());

    //Barrel charge - normalised
    TCanvas c3("c3","", 850, 500);
    c3.SetGrid();
    TLegend *leg3 = new TLegend(0.75,0.8,0.9,0.9);
    TGraphErrors *barQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &barrelVec[0], 0, &errorBarVec[0]);
    format_plots(barQ1, leg3, 1.37, 1.55,"%m-%d","barrelQ/totQ",tempNameBar+periodTitle);
    std::string printname3 = outname + "_barQ" + periodFileEnd + ".png";
    c3.Print(printname3.c_str());

    //Barrel z&y > 0 - normalised
    TCanvas c4("c4","", 850, 500);
    c4.SetGrid();
    TLegend *leg4 = new TLegend(0.75,0.8,0.9,0.9);
    TGraphErrors *bzyQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bzyVec[0], 0, &errorBzyVec[0]);
    format_plots(bzyQ1, leg4, 1.37, 1.55,"%m-%d","bz0y0Q/totQ",tempNameBzy+periodTitle);
    std::string printname4 = outname + "_bzyQ" + periodFileEnd + ".png";
    c4.Print(printname4.c_str());

    //Barrel z&y < 0 - normalised
    TCanvas c5("c5", "", 850, 500);
    c5.SetGrid();
    TLegend *leg5 = new TLegend(0.75, 0.8, 0.9, 0.9);
    TGraphErrors *bzylt0Q1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bzylt0Vec[0], 0, &errorBzylt0Vec[0]);
    format_plots(bzylt0Q1, leg5, 0.8, 1.2, "%m-%d", "bzylt0Q/totQ", tempNameBzylt0+periodTitle);
    std::string printname5 = outname + "_bzylt0Q" + periodFileEnd + ".png";
    c5.Print(printname5.c_str());

    //Barrel z&y<0 / z&y>0 (longest/shortest) - normalised
    TCanvas c6("c6", "", 850, 500);
    c6.SetGrid();
    TLegend *leg6 = new TLegend(0.75, 0.8, 0.9, 0.9);
    TGraphErrors *bzyRatio = new TGraphErrors(dateVec.size(), &dateVec[0], &bzyRatioVec[0], 0, &errorBzyRatioVec[0]);
    format_plots(bzyRatio, leg6, 0.8, 1.2, "%m-%d", "longest/shortest", tempNameBzyRatio+periodTitle);
    std::string printname6 = outname + "_bzyRatioQ" + periodFileEnd + ".png";
    c6.Print(printname6.c_str());

  }//end of loop over different periods
  
}




void extract_data(std::ifstream &file,std::vector<double> &totQ, std::vector<double> &barrelQ,std::vector<int> &year,std::vector<int> &month,std::vector<int> &day,std::vector<int> &hour,std::vector<int> &minute,std::vector<int> &second,std::vector<double> &bottomQ,std::vector<double> &bz0y0Q,std::vector<double> &bzylt0Q){

  std::string line;
  getline(file,line);

  while(getline(file, line)){

    std::stringstream line_stream(line);
    std::string entry;
    std::vector<std::string> entries;
    char delim = ' ';
    while(getline(line_stream, entry, delim)){
      entries.push_back(entry);
    }
    year.push_back(std::stoi(entries.at(2)));
    month.push_back(std::stoi(entries.at(3)));
    day.push_back(std::stoi(entries.at(4)));
    hour.push_back(std::stoi(entries.at(5)));
    minute.push_back(std::stoi(entries.at(6)));
    second.push_back(std::stoi(entries.at(7)));
    totQ.push_back(std::stod(entries.at(8)));
    barrelQ.push_back(std::stod(entries.at(9)));
    bz0y0Q.push_back(std::stod(entries.at(10)));
    bottomQ.push_back(std::stod(entries.at(11)));
    bzylt0Q.push_back(std::stod(entries.at(12)));
  }
}


void format_plots(TGraph *graph1,TLegend *leg,float min, float max, std::string timeform, std::string yaxis, std::string title){

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
  //  setMinMax(graph1);
  graph1->GetXaxis()->SetTimeDisplay(1);
  graph1->GetXaxis()->SetTimeFormat(timeform.c_str());
  graph1->GetXaxis()->SetTimeOffset(0,"jst");

  leg->SetBorderSize(0);
  leg->SetNColumns(5);
  leg->SetFillColor(0);
  leg->AddEntry(graph1, "B1", "p" );
  //leg->Draw();
  
}


void format_plots_multi(TGraph *graph1, TGraph *graph2, TLegend *leg,float min, float max, std::string timeform, std::string yaxis, std::string title){

  TDatime now;

  title = title + "(" + now.GetDate() + ")";
  
  graph1->SetMarkerStyle(20);
  graph1->SetMarkerColor(2);
  graph1->SetLineColor(2);
  graph1->SetMarkerSize(0.4);
  graph2->SetMarkerStyle(20);
  graph2->SetMarkerColor(4);
  graph2->SetLineColor(4);
  graph2->SetMarkerSize(0.4);
  graph1->Draw("aZpsame");
  graph2->Draw("Zpsame");
  graph1->SetFillColor(0);
  graph1->GetYaxis()->SetTitle(yaxis.c_str());
  graph1->SetTitle(title.c_str());
  graph1->GetYaxis()->SetRangeUser(min,max);
  graph1->GetXaxis()->SetTimeDisplay(1);
  graph1->GetXaxis()->SetTimeFormat(timeform.c_str());
  graph1->GetXaxis()->SetTimeOffset(0,"jst");

  //leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(graph1, "All Barrel", "p" );
  leg->AddEntry(graph2, "z > 0 && y > 0", "p" );
  leg->Draw();
  
}



void normalisation(std::vector<float> &in, float normVal, std::vector<float> &error){

  float prevError;
  for (int i =0; i < in.size(); i++){
    prevError = in[i];
    in[i] = in[i]/normVal;
    error[i] = error[i]*in[i]/prevError;
  }

}


void setMinMax(TGraph *g){

  double min, max;
  //min = TMath::MinElement(g->GetN(), g->GetY());
  //max = TMath::MaxElement(g->GetN(), g->GetY());
  const long long N = g->GetN();
  long long ind[N];
  //Sort smallest to largest
  TMath::Sort(N, g->GetY(), ind, false);

  min = g->GetY()[ind[0]];
  max = g->GetY()[ind[N-1]];
  if(max > 1.003*g->GetY()[ind[N-2]]) max = g->GetY()[ind[N-2]];
  if(min < 0.998*g->GetY()[ind[1]])   min = g->GetY()[ind[1]];
  
  g->GetYaxis()->SetRangeUser(0.999*min, 1.001*max);

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
