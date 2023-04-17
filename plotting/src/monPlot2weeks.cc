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

void extract_data(std::ifstream &file,std::vector<int> &month,std::vector<int> &day,std::vector<int> &hour,std::vector<int> &minute,std::vector<int> &second, std::vector<double> &totQ, std::vector<double> &monQ);
void format_plots(TGraph *graph1,TGraph *graph2,TGraph *graph3,TGraph *graph4,TGraph *graph5,TLegend *leg ,float min, float max, std::string timeform, std::string yaxis, std::string title);

int main(int argc, char *argv[]){

  std::string beamType = "dif";
  int oNameSwitch = 0;
  std::string ldir = "/disk02/usr6/jmcelwee/monitoringData/plotting";
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

  std::vector<std::vector<float>> dateVecVec;
  std::vector<std::vector<float>> monQVecVec, errorMonVecVec;
  std::vector<std::vector<float>> normQVecVec, errorNormVecVec;
  
  for (int inj=0; inj<5;inj++){
    std::vector<int> run, subrun, year, month, day, hour, minute, second;
    std::vector<double> monQ, normQ,totQ;
    std::ifstream file(fileList[inj]);
    std::cout << fileList[inj] << std::endl;
    extract_data(file,month,day,hour,minute,second,totQ,monQ);
    
    int avenum = 1500;
    std::vector<float> dateVec;
    std::vector<float> monDataPoints, monQVec, errorMonVec;
    std::vector<float> normDataPoints, normQVec, errorNormVec;
    float averageNorm = 0;
    float averageMon = 0;
    int count = 0;
    float errorMon, errorNorm;
    
    int time = timer - 3*604800;
    //    int time = timer - 2*2997000;

    int countMon = 0;

    
    for (int i=0; i<monQ.size(); i++){
      TDatime date(2020,month.at(i),day.at(i),hour.at(i),minute.at(i),second.at(i));

      if (monQ.at(i) > 500 && monQ.at(i) < 4000 && date.Convert() > time){
	if (i == 0){

	  if (totQ.at(i) < 2000){
	    count++;
	    
	    monDataPoints.push_back(monQ.at(i));
	    averageMon += monQ.at(i);
	    countMon++;
	    
	    normDataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm += totQ.at(i)/monQ.at(i);
	  }
	}
	else if (i % avenum == 0){

	  if (totQ.at(i) < 2000){
	    count++;
	    // Average Calculation
	    
	    monDataPoints.push_back(monQ.at(i));
	    averageMon += monQ.at(i);
	    countMon++;
	    
	    normDataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm += totQ.at(i)/monQ.at(i);
	  }
	  
	  averageMon /= countMon;
	  averageNorm /= countMon;
	  
	  float maxmon = *std::max_element(monDataPoints.begin(),monDataPoints.end());
	  float minmon = *std::min_element(monDataPoints.begin(),monDataPoints.end());
	  errorMon = (maxmon - minmon) / (2 * std::sqrt(countMon));
	  errorMonVec.push_back(errorMon);
	  monDataPoints.clear();

	  float maxnorm = *std::max_element(normDataPoints.begin(),normDataPoints.end());
	  float minnorm = *std::min_element(normDataPoints.begin(),normDataPoints.end());
	  errorNorm = (maxnorm - minnorm) / (2 * std::sqrt(countMon));
	  errorNormVec.push_back(errorNorm);
	  normDataPoints.clear();
	  
	  // Sort out average vectors
	  dateVec.push_back(date.Convert());
	  count = 0;
	  monQVec.push_back(averageMon);
	  normQVec.push_back(averageNorm);
	  averageMon = 0;
	  averageNorm = 0;
	  countMon = 0;
	  
	}
	else {
	  if (totQ.at(i) < 2000){
	    count++;
	    
	    monDataPoints.push_back(monQ.at(i));
	    averageMon += monQ.at(i);
	    countMon++;
	    
	    normDataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm += totQ.at(i)/monQ.at(i);
	  }
	}
      
      }
    
    }

    dateVecVec.push_back(dateVec);

    monQVecVec.push_back(monQVec);
    errorMonVecVec.push_back(errorMonVec);

    normQVecVec.push_back(normQVec);
    errorNormVecVec.push_back(errorNormVec);

    
  }
  


  if (oNameSwitch == 0) outname = "/home/jmcelwee/monitoring/plotting/" + beamType;
  std::string tempNameMon = "Mon Q (" + beamType + ") ";
  std::string tempNameNorm = "Norm Q (" + beamType + ") ";
  

  TCanvas c7("c7","");
  c7.SetGrid();
  TLegend *leg7 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *monQ1 = new TGraphErrors(dateVecVec[0].size(), &dateVecVec[0][0], &monQVecVec[0][0], 0, &errorMonVecVec[0][0]);
  TGraphErrors *monQ2 = new TGraphErrors(dateVecVec[1].size(), &dateVecVec[1][0], &monQVecVec[1][0], 0, &errorMonVecVec[1][0]);
  TGraphErrors *monQ3 = new TGraphErrors(dateVecVec[2].size(), &dateVecVec[2][0], &monQVecVec[2][0], 0, &errorMonVecVec[2][0]);
  TGraphErrors *monQ4 = new TGraphErrors(dateVecVec[3].size(), &dateVecVec[3][0], &monQVecVec[3][0], 0, &errorMonVecVec[3][0]);
  TGraphErrors *monQ5 = new TGraphErrors(dateVecVec[4].size(), &dateVecVec[4][0], &monQVecVec[4][0], 0, &errorMonVecVec[4][0]);
  format_plots(monQ1, monQ2, monQ3, monQ4, monQ5,leg7, 2870, 2970, "%m-%d","Mon Q",tempNameMon+" 3 Weeks");
  std::string printname7 = outname + "_monQ_2weeks.png";
  c7.Print(printname7.c_str());
  
  TCanvas c1("c1","");
  c1.SetGrid();
  TLegend *leg1 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *normQ1 = new TGraphErrors(dateVecVec[0].size(), &dateVecVec[0][0], &normQVecVec[0][0], 0, &errorNormVecVec[0][0]);
  TGraphErrors *normQ2 = new TGraphErrors(dateVecVec[1].size(), &dateVecVec[1][0], &normQVecVec[1][0], 0, &errorNormVecVec[1][0]);
  TGraphErrors *normQ3 = new TGraphErrors(dateVecVec[2].size(), &dateVecVec[2][0], &normQVecVec[2][0], 0, &errorNormVecVec[2][0]);
  TGraphErrors *normQ4 = new TGraphErrors(dateVecVec[3].size(), &dateVecVec[3][0], &normQVecVec[3][0], 0, &errorNormVecVec[3][0]);
  TGraphErrors *normQ5 = new TGraphErrors(dateVecVec[4].size(), &dateVecVec[4][0], &normQVecVec[4][0], 0, &errorNormVecVec[4][0]);
  format_plots(normQ1, normQ2, normQ3, normQ4, normQ5,leg1, 0, 0.3, "%m-%d","Norm Q",tempNameNorm+" 3 Weeks");
  std::string printname1 = outname + "_normQ_2weeks.png";
  c1.Print(printname1.c_str());
  
  return 0;
}




void extract_data(std::ifstream &file,std::vector<int> &month,std::vector<int> &day,std::vector<int> &hour,std::vector<int> &minute,std::vector<int> &second,std::vector<double> &totQ,std::vector<double> &monQ){

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
    //spotQ.push_back(std::stof(entries.at(11)));
    monQ.push_back(std::stod(entries.back()));
  }
}


void format_plots(TGraph *graph1,TGraph *graph2,TGraph *graph3,TGraph *graph4,TGraph *graph5, TLegend *leg,float min, float max, std::string timeform, std::string yaxis, std::string title){

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
