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

void extract_data(std::ifstream &file, std::vector<float> &totQ,std::vector<float> &spotQ,std::vector<int> &month,std::vector<int> &day,std::vector<int> &hour,std::vector<int> &minute,std::vector<int> &second);
void format_plots(TGraph *graph1,TGraph *graph2,TGraph *graph3,TGraph *graph4,TGraph *graph5,TLegend *leg ,int min, int max, std::string timeform, std::string yaxis, std::string title);

int main(int argc, char *argv[]){

  std::string beamType = "dif";
  int oNameSwitch = 0;
  std::string ldir = "/disk02/calib3/usr/ukli/monitoring/plotting";
  std::string endname = "";
  std::string outname = "";
  int opt;
  while ((opt = getopt(argc, argv, ":dco:l:")) != -1){
    switch (opt)
      {
      case 'd':
	beamType = "dif";
	break;
      case 'c':
	beamType = "col";
	break;
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
  
  std::vector<std::string> fileList;
  for (int inj=0; inj<5;inj++){
    int injcount = inj+1;
    std::string fileEx = ldir + "/" +  beamType  + "_B" + std::to_string(inj+1) + ".dat";
    fileList.push_back(fileEx);
    std::cout << fileEx << std::endl;
  }

  
  time_t timer;
  time(&timer);

  gStyle->SetCanvasColor(0);
  gStyle->SetTitleColor(0);
  gStyle->SetOptStat(0);  

  std::vector<std::vector<float>> dateVecVec, totQVecVec, spotQVecVec, date6VecVec,totQ6VecVec,spotQ6VecVec, date24VecVec,totQ24VecVec,spotQ24VecVec,errorTotVecVec,errorTot6VecVec,errorTot24VecVec,errorSpotVecVec,errorSpot6VecVec,errorSpot24VecVec;

  
  for (int inj=0; inj<5;inj++){
    std::vector<int> run, subrun, year, month, day, hour, minute, second;
    std::vector<float> totQ, spotQ;
    std::ifstream file(fileList[inj]);
    std::cout << fileList[inj] << std::endl;
    extract_data(file,totQ,spotQ,month,day,hour,minute,second);
    
    int avenum = 4000;
    std::vector<float> totDataPoints, totQVec, spotDataPoints, spotQVec, dateVec, errorSpotVec, errorTotVec;
    float averageTot = 0;
    float averageSpot = 0;
    int count = 0;

    
    int time6 = timer - 21600;
    int time24 = timer - 86400;
    std::vector<float> totQ6Vec, spotQ6Vec, date6Vec, totQ24Vec, spotQ24Vec, date24Vec, errorTot6Vec,errorTot24Vec, errorSpot6Vec, errorSpot24Vec, tot24DataPoints, spot24DataPoints, tot6DataPoints, spot6DataPoints;
    int avenum24 = 100;
    float averageTot24 = 0;
    float averageSpot24 = 0;
    int count24 = 0;
    int avenum6 = 20;
    float averageTot6 = 0;
    float averageSpot6 = 0;
    int count6 = 0;
    float errorTot,errorSpot,errorTot6,errorSpot6,errorTot24,errorSpot24;
    
    for (int i=0; i<totQ.size(); i++){
      TDatime date(2020,month.at(i),day.at(i),hour.at(i),minute.at(i),second.at(i));
      if (i == 0){
	count++;
	averageTot += totQ.at(i);
	averageSpot += spotQ.at(i);
	if (totQ.at(i) < 2000){
	  totDataPoints.push_back(totQ.at(i));
	}
	if (spotQ.at(i) < 2000){
	  spotDataPoints.push_back(spotQ.at(i));
	}
      }
      else if (i % avenum == 0){
	count++;
	// Average Calculation
	averageTot += totQ.at(i);
	averageTot /= count;
	averageSpot += spotQ.at(i);
	averageSpot /= count;

	if (totQ.at(i) < 2000){
	  totDataPoints.push_back(totQ.at(i));
	}
	if (spotQ.at(i) < 2000){
	  spotDataPoints.push_back(spotQ.at(i));
	}
	  
	float max = *std::max_element(totDataPoints.begin(),totDataPoints.end());
	float min = *std::min_element(totDataPoints.begin(),totDataPoints.end());
	errorTot = (max - min) / (2 * std::sqrt(count));
	//std::cout << max << " " << min << std::endl;
	float maxsp = *std::max_element(spotDataPoints.begin(),spotDataPoints.end());
	float minsp = *std::min_element(spotDataPoints.begin(),spotDataPoints.end());
	errorSpot = (maxsp - minsp) / (2 * std::sqrt(count));
	errorTotVec.push_back(errorTot);
	errorSpotVec.push_back(errorSpot);
	totDataPoints.clear();
	spotDataPoints.clear();
	
	// Sort out average vectors
	totQVec.push_back(averageTot);
	spotQVec.push_back(averageSpot);
	dateVec.push_back(date.Convert());
	averageTot = 0;
	averageSpot = 0;
	count = 0;
      }
      else {
	averageTot += totQ.at(i);
	averageSpot += spotQ.at(i);
	count++;
	if (totQ.at(i) < 2000){
	  totDataPoints.push_back(totQ.at(i));
	}
	if (spotQ.at(i) < 2000){
	  spotDataPoints.push_back(spotQ.at(i));
	}
      }

      
      if (date.Convert() > time24) {
	if (i == 0){
	  count24++;
	  averageTot24 += totQ.at(i);
	  averageSpot24 += spotQ.at(i);
	  tot24DataPoints.push_back(totQ.at(i));
	  spot24DataPoints.push_back(spotQ.at(i));
	}
	else if (i % avenum24 == 0){
	  count24++;
	  // Average Calculation
	  averageTot24 += totQ.at(i);
	  averageTot24 /= count24;
	  averageSpot24 += spotQ.at(i);
	  averageSpot24 /= count24;

	  tot24DataPoints.push_back(totQ.at(i));
	  spot24DataPoints.push_back(spotQ.at(i));
	  
	  float max24 = *std::max_element(tot24DataPoints.begin(),tot24DataPoints.end());
	  float min24 = *std::min_element(tot24DataPoints.begin(),tot24DataPoints.end());
	  errorTot24 = (max24 - min24) / (2 * std::sqrt(count24));
	  //	  std::cout << errorTot24 << std::endl;
	  errorSpot24 = (std::max_element(spot24DataPoints.begin(),spot24DataPoints.end()) - std::min_element(spot24DataPoints.begin(),spot24DataPoints.end())) / (2 * std::sqrt(count24));
	  errorTot24Vec.push_back(errorTot24);
	  errorSpot24Vec.push_back(errorSpot24);
	  tot24DataPoints.clear();
	  spot24DataPoints.clear();
	  
	  
	  // Sort out average vectors
	  totQ24Vec.push_back(averageTot24);
	  spotQ24Vec.push_back(averageSpot24);
	  date24Vec.push_back(date.Convert());
	  averageTot24 = 0;
	  averageSpot24 = 0;
	  count24 = 0;
	}
	else {
	  averageTot24 += totQ.at(i);
	  averageSpot24 += spotQ.at(i);
	  count24++;
	  tot24DataPoints.push_back(totQ.at(i));
	  spot24DataPoints.push_back(spotQ.at(i));
	}
	
      }


      if (date.Convert() > time6) {
	if (i == 0){
	  count6++;
	  averageTot6 += totQ.at(i);
	  averageSpot6 += spotQ.at(i);
	  tot6DataPoints.push_back(totQ.at(i));
	  spot6DataPoints.push_back(spotQ.at(i));
	}
	else if (i % avenum6 == 0){
	  count6++;
	  // Average Calculation
	  averageTot6 += totQ.at(i);
	  averageTot6 /= count6;
	  averageSpot6 += spotQ.at(i);
	  averageSpot6 /= count6;

	  
	  tot6DataPoints.push_back(totQ.at(i));
	  spot6DataPoints.push_back(spotQ.at(i));
	  
	  float max6 = *std::max_element(tot6DataPoints.begin(),tot6DataPoints.end());
	  float min6 = *std::min_element(tot6DataPoints.begin(),tot6DataPoints.end());
	  errorTot6 = (max6 - min6) / (2 * std::sqrt(count6));
	  //	  std::cout << errorTot6 << std::endl;
	  errorSpot6 = (std::max_element(spot6DataPoints.begin(),spot6DataPoints.end()) - std::min_element(spot6DataPoints.begin(),spot6DataPoints.end())) / (2 * std::sqrt(count6));
	  errorTot6Vec.push_back(errorTot6);
	  errorSpot6Vec.push_back(errorSpot6);
	  tot6DataPoints.clear();
	  spot6DataPoints.clear();
	  

	  
	  // Sort out average vectors
	  totQ6Vec.push_back(averageTot6);
	  spotQ6Vec.push_back(averageSpot6);
	  date6Vec.push_back(date.Convert());
	  averageTot6 = 0;
	  averageSpot6 = 0;
	  count6 = 0;
	}
	else {
	  averageTot6 += totQ.at(i);
	  averageSpot6 += spotQ.at(i);
	  count6++;
	  tot6DataPoints.push_back(totQ.at(i));
	  spot6DataPoints.push_back(spotQ.at(i));
	}
	
      }
      
    }
    
    
    errorTotVecVec.push_back(errorTotVec);
    errorTot6VecVec.push_back(errorTot6Vec);
    errorTot24VecVec.push_back(errorTot24Vec);
    errorSpotVecVec.push_back(errorSpotVec);
    errorSpot6VecVec.push_back(errorSpot6Vec);
    errorSpot24VecVec.push_back(errorSpot24Vec);
    date6VecVec.push_back(date6Vec);
    totQ6VecVec.push_back(totQ6Vec);
    spotQ6VecVec.push_back(spotQ6Vec);
    date24VecVec.push_back(date24Vec);
    totQ24VecVec.push_back(totQ24Vec);
    spotQ24VecVec.push_back(spotQ24Vec);
    dateVecVec.push_back(dateVec);
    totQVecVec.push_back(totQVec);
    spotQVecVec.push_back(spotQVec);
  }
  


  if (oNameSwitch == 0) outname = "/home/jmcelwee/monitoring/plotting/" + beamType;
  std::string tempNameTot = "Total Q (" + beamType + ") ";
  std::string tempNameSpot = "Spot Q (" + beamType + ") ";
  
  TCanvas c1("c1","");
  c1.SetGrid();
  TLegend *leg1 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *totQ1 = new TGraphErrors(dateVecVec[0].size(), &dateVecVec[0][0], &totQVecVec[0][0], 0, &errorTotVecVec[0][0]);
  TGraphErrors *totQ2 = new TGraphErrors(dateVecVec[1].size(), &dateVecVec[1][0], &totQVecVec[1][0], 0, &errorTotVecVec[1][0]);
  TGraphErrors *totQ3 = new TGraphErrors(dateVecVec[2].size(), &dateVecVec[2][0], &totQVecVec[2][0], 0, &errorTotVecVec[2][0]);
  TGraphErrors *totQ4 = new TGraphErrors(dateVecVec[3].size(), &dateVecVec[3][0], &totQVecVec[3][0], 0, &errorTotVecVec[3][0]);
  TGraphErrors *totQ5 = new TGraphErrors(dateVecVec[4].size(), &dateVecVec[4][0], &totQVecVec[4][0], 0, &errorTotVecVec[4][0]);
  format_plots(totQ1, totQ2, totQ3, totQ4, totQ5,leg1, 0, 580, "%m-%d","Tot Q (p.e)",tempNameTot+"All time");
  std::string printname1 = outname + "_totQ.png";
  c1.Print(printname1.c_str());
  
  TCanvas c3("c3","");
  c3.SetGrid();
  TLegend *leg3 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *totQ61 = new TGraphErrors(date6VecVec[0].size(), &date6VecVec[0][0], &totQ6VecVec[0][0], 0, &errorTot6VecVec[0][0]);
  TGraphErrors *totQ62 = new TGraphErrors(date6VecVec[1].size(), &date6VecVec[1][0], &totQ6VecVec[1][0], 0, &errorTot6VecVec[1][0]);
  TGraphErrors *totQ63 = new TGraphErrors(date6VecVec[2].size(), &date6VecVec[2][0], &totQ6VecVec[2][0], 0, &errorTot6VecVec[2][0]);
  TGraphErrors *totQ64 = new TGraphErrors(date6VecVec[3].size(), &date6VecVec[3][0], &totQ6VecVec[3][0], 0, &errorTot6VecVec[3][0]);
  TGraphErrors *totQ65 = new TGraphErrors(date6VecVec[4].size(), &date6VecVec[4][0], &totQ6VecVec[4][0], 0, &errorTot6VecVec[4][0]);
  format_plots(totQ61, totQ62, totQ63,totQ64,totQ65,leg3,0,580,"%H:%M","Tot Q (p.e)",tempNameTot+"Past 6h");
  std::string printname3 = outname + "_totQ_six.png";
  c3.Print(printname3.c_str());

  TCanvas c5("c5","");
  c5.SetGrid();
  TLegend *leg5 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *totQ241 = new TGraphErrors(date24VecVec[0].size(), &date24VecVec[0][0], &totQ24VecVec[0][0], 0, &errorTot24VecVec[0][0]);
  TGraphErrors *totQ242 = new TGraphErrors(date24VecVec[1].size(), &date24VecVec[1][0], &totQ24VecVec[1][0], 0, &errorTot24VecVec[1][0]);
  TGraphErrors *totQ243 = new TGraphErrors(date24VecVec[2].size(), &date24VecVec[2][0], &totQ24VecVec[2][0], 0, &errorTot24VecVec[2][0]);
  TGraphErrors *totQ244 = new TGraphErrors(date24VecVec[3].size(), &date24VecVec[3][0], &totQ24VecVec[3][0], 0, &errorTot24VecVec[3][0]);
  TGraphErrors *totQ245 = new TGraphErrors(date24VecVec[4].size(), &date24VecVec[4][0], &totQ24VecVec[4][0], 0, &errorTot24VecVec[4][0]);
  format_plots(totQ241, totQ242, totQ243, totQ244, totQ245,leg5, 0, 580,"%H:%M","Tot Q (p.e)",tempNameTot+"Past 24h");
  std::string printname5 = outname + "_totQ_day.png";
  c5.Print(printname5.c_str());

  
  TCanvas c2("c2","");
  c2.SetGrid();
  TLegend *leg2 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *spotQ1 = new TGraphErrors(dateVecVec[0].size(), &dateVecVec[0][0], &spotQVecVec[0][0], 0, &errorSpotVecVec[0][0]);
  TGraphErrors *spotQ2 = new TGraphErrors(dateVecVec[1].size(), &dateVecVec[1][0], &spotQVecVec[1][0], 0, &errorSpotVecVec[1][0]);
  TGraphErrors *spotQ3 = new TGraphErrors(dateVecVec[2].size(), &dateVecVec[2][0], &spotQVecVec[2][0], 0, &errorSpotVecVec[2][0]);
  TGraphErrors *spotQ4 = new TGraphErrors(dateVecVec[3].size(), &dateVecVec[3][0], &spotQVecVec[3][0], 0, &errorSpotVecVec[3][0]);
  TGraphErrors *spotQ5 = new TGraphErrors(dateVecVec[4].size(), &dateVecVec[4][0], &spotQVecVec[4][0], 0, &errorSpotVecVec[4][0]);
  format_plots(spotQ1, spotQ2, spotQ3, spotQ4, spotQ5,leg2, 0, 400,"%m-%d","Spot Q (p.e)",tempNameSpot+"All time");
  std::string printname2 = outname + "_spotQ.png";
  c2.Print(printname2.c_str());
  
  TCanvas c4("c4","");
  c4.SetGrid();
  TLegend *leg4 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *spotQ61 = new TGraphErrors(date6VecVec[0].size(), &date6VecVec[0][0], &spotQ6VecVec[0][0], 0, &errorSpot6VecVec[0][0]);
  TGraphErrors *spotQ62 = new TGraphErrors(date6VecVec[1].size(), &date6VecVec[1][0], &spotQ6VecVec[1][0], 0, &errorSpot6VecVec[1][0]);
  TGraphErrors *spotQ63 = new TGraphErrors(date6VecVec[2].size(), &date6VecVec[2][0], &spotQ6VecVec[2][0], 0, &errorSpot6VecVec[2][0]);
  TGraphErrors *spotQ64 = new TGraphErrors(date6VecVec[3].size(), &date6VecVec[3][0], &spotQ6VecVec[3][0], 0, &errorSpot6VecVec[3][0]);
  TGraphErrors *spotQ65 = new TGraphErrors(date6VecVec[4].size(), &date6VecVec[4][0], &spotQ6VecVec[4][0], 0, &errorSpot6VecVec[4][0]);
  format_plots(spotQ61, spotQ62, spotQ63, spotQ64, spotQ65,leg4, 0, 400,"%H:%M","Spot Q (p.e)",tempNameSpot+"Past 6h");
  std::string printname4 = outname + "_spotQ_six.png";
  c4.Print(printname4.c_str());
  
  TCanvas c6("c6","");
  c6.SetGrid();
  TLegend *leg6 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *spotQ241 = new TGraphErrors(date24VecVec[0].size(), &date24VecVec[0][0], &spotQ24VecVec[0][0], 0, &errorSpot24VecVec[0][0]);
  TGraphErrors *spotQ242 = new TGraphErrors(date24VecVec[1].size(), &date24VecVec[1][0], &spotQ24VecVec[1][0], 0, &errorSpot24VecVec[1][0]);
  TGraphErrors *spotQ243 = new TGraphErrors(date24VecVec[2].size(), &date24VecVec[2][0], &spotQ24VecVec[2][0], 0, &errorSpot24VecVec[2][0]);
  TGraphErrors *spotQ244 = new TGraphErrors(date24VecVec[3].size(), &date24VecVec[3][0], &spotQ24VecVec[3][0], 0, &errorSpot24VecVec[3][0]);
  TGraphErrors *spotQ245 = new TGraphErrors(date24VecVec[4].size(), &date24VecVec[4][0], &spotQ24VecVec[4][0], 0, &errorSpot24VecVec[4][0]);
  format_plots(spotQ241, spotQ242, spotQ243, spotQ244, spotQ245,leg6, 0, 400,"%H:%M","Spot Q (p.e)",tempNameSpot+"Past 24h");
  std::string printname6 = outname + "_spotQ_day.png";
  c6.Print(printname6.c_str());

  return 0;
}




void extract_data(std::ifstream &file,std::vector<float> &totQ, std::vector<float> &spotQ,std::vector<int> &month,std::vector<int> &day,std::vector<int> &hour,std::vector<int> &minute,std::vector<int> &second){

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
    month.push_back(std::stoi(entries.at(3)));
    day.push_back(std::stoi(entries.at(4)));
    hour.push_back(std::stoi(entries.at(5)));
    minute.push_back(std::stoi(entries.at(6)));
    second.push_back(std::stoi(entries.at(7)));
    totQ.push_back(std::stof(entries.at(10)));
    spotQ.push_back(std::stof(entries.at(11)));
    //monQ.push_back(std::stof(entries.at(12)));
  }
}


void format_plots(TGraph *graph1,TGraph *graph2,TGraph *graph3,TGraph *graph4,TGraph *graph5, TLegend *leg,int min, int max, std::string timeform, std::string yaxis, std::string title){

  TDatime now;

  title = title + "(" + now.GetDate() + ")";
  
  graph1->SetMarkerStyle(20);
  graph2->SetMarkerStyle(20);
  graph3->SetMarkerStyle(20);
  graph4->SetMarkerStyle(20);
  graph5->SetMarkerStyle(20);
  graph1->SetMarkerColor(2);
  graph1->SetLineColor(2);
  graph2->SetMarkerColor(3);
  graph2->SetLineColor(3);
  graph3->SetMarkerColor(4);
  graph3->SetLineColor(4);
  graph4->SetMarkerColor(6);
  graph4->SetLineColor(6);
  graph5->SetMarkerColor(7);
  graph5->SetLineColor(7);
  graph1->SetMarkerSize(0.4);
  graph2->SetMarkerSize(0.4);
  graph3->SetMarkerSize(0.4);
  graph4->SetMarkerSize(0.4);
  graph5->SetMarkerSize(0.4);
  graph1->Draw("aZpsame");
  graph1->SetFillColor(0);
  graph1->GetYaxis()->SetTitle(yaxis.c_str());
  graph1->SetTitle(title.c_str());
  graph1->GetYaxis()->SetRangeUser(min,max);
  graph2->Draw("Zpsame");
  graph3->Draw("Zpsame");
  graph4->Draw("Zpsame");
  graph5->Draw("Zpsame");
  graph1->GetXaxis()->SetTimeDisplay(1);
  graph1->GetXaxis()->SetTimeFormat(timeform.c_str());
  graph1->GetXaxis()->SetTimeOffset(0,"jst");

  leg->SetBorderSize(0);
  leg->SetNColumns(5);
  leg->SetFillColor(0);
  leg->AddEntry(graph1, "B1", "p" );
  leg->AddEntry(graph2, "B2", "p" );
  leg->AddEntry(graph3, "B3", "p" );
  leg->AddEntry(graph4, "B4", "p" );
  leg->AddEntry(graph5, "B5", "p" );
  leg->Draw();
  
}
