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

  std::vector<std::vector<float>> dateVecVec, date6VecVec, date24VecVec;
  std::vector<std::vector<float>> monQVecVec, errorMonVecVec, monQ24VecVec, errorMon24VecVec, monQ6VecVec, errorMon6VecVec;
  std::vector<std::vector<float>> normQVecVec, errorNormVecVec, normQ24VecVec, errorNorm24VecVec, normQ6VecVec, errorNorm6VecVec;
  
  for (int inj=0; inj<5;inj++){
    std::vector<int> run, subrun, year, month, day, hour, minute, second;
    std::vector<double> monQ, normQ,totQ;
    std::ifstream file(fileList[inj]);
    std::cout << fileList[inj] << std::endl;
    extract_data(file,month,day,hour,minute,second,totQ,monQ);
    
    int avenum = 3000;
    std::vector<float> dateVec;
    std::vector<float> monDataPoints, monQVec, errorMonVec;
    std::vector<float> normDataPoints, normQVec, errorNormVec;
    float averageNorm = 0;
    float averageMon = 0;
    int count = 0;

    
    int time6 = timer - 21600;
    int time24 = timer - 86400;
    std::vector<float> date6Vec, date24Vec;
    std::vector<float> monQ6Vec, monQ24Vec, mon6DataPoints, mon24DataPoints, errorMon6Vec, errorMon24Vec;
    std::vector<float> normQ6Vec, normQ24Vec, norm6DataPoints, norm24DataPoints, errorNorm6Vec, errorNorm24Vec;

    int avenum24 = 100;
    int count24 = 0;
    int avenum6 = 20;
    int count6 = 0;
    float errorMon, errorMon6, errorMon24;
    float errorNorm, errorNorm6, errorNorm24;


    int countMon = 0;
    float averageMon6 = 0;
    float averageMon24 = 0;
    float averageNorm6 = 0;
    float averageNorm24 = 0;
    
    for (int i=0; i<monQ.size(); i++){
      TDatime date(2020,month.at(i),day.at(i),hour.at(i),minute.at(i),second.at(i));

      if (monQ.at(i) > 500 && monQ.at(i) < 4000){
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

      
	if (date.Convert() > time24) {
	  if (i == 0){
	    count24++;

	    mon24DataPoints.push_back(monQ.at(i));
	    averageMon24 += monQ.at(i);
	    
	    norm24DataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm24 += totQ.at(i)/monQ.at(i);	    
	  }
	  else if (i % avenum24 == 0){
	    count24++;

	    mon24DataPoints.push_back(monQ.at(i));
	    averageMon24 += monQ.at(i);

	    norm24DataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm24 += totQ.at(i)/monQ.at(i);
	    
	    averageMon24 /= count24;
	    averageNorm24 /= count24;
	    
	    float maxmon24 = *std::max_element(mon24DataPoints.begin(),mon24DataPoints.end());
	    float minmon24 = *std::min_element(mon24DataPoints.begin(),mon24DataPoints.end());
	    errorMon24 = (maxmon24 - minmon24) / (2 * std::sqrt(count24));
	    errorMon24Vec.push_back(errorMon24);
	    mon24DataPoints.clear();

	    float maxnorm24 = *std::max_element(norm24DataPoints.begin(),norm24DataPoints.end());
	    float minnorm24 = *std::min_element(norm24DataPoints.begin(),norm24DataPoints.end());
	    errorNorm24 = (maxnorm24 - minnorm24) / (2 * std::sqrt(count24));
	    errorNorm24Vec.push_back(errorNorm24);
	    norm24DataPoints.clear();
	    
	    
	    // Sort out average vectors
	    monQ24Vec.push_back(averageMon24);
	    normQ24Vec.push_back(averageNorm24);
	    date24Vec.push_back(date.Convert());
	    averageMon24 = 0;
	    averageNorm24 = 0;
	    count24 = 0;
	  }
	  else {
	    count24++;
	    mon24DataPoints.push_back(monQ.at(i));
	    averageMon24 += monQ.at(i);
	    norm24DataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm24 += totQ.at(i)/monQ.at(i);
	  }
	
	}


	if (date.Convert() > time6) {
	  if (i == 0){
	    count6++;
	    mon6DataPoints.push_back(monQ.at(i));
	    averageMon6 += monQ.at(i);
	    norm6DataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm6 += totQ.at(i)/monQ.at(i);
	  }
	  else if (i % avenum6 == 0){
	    count6++;

	    mon6DataPoints.push_back(monQ.at(i));
	    averageMon6 += monQ.at(i);
	    
	    norm6DataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm6 += totQ.at(i)/monQ.at(i);
	    
	    averageMon6 /= count6;
	    averageNorm6 /= count6;
	    
	    float maxmon6 = *std::max_element(mon6DataPoints.begin(),mon6DataPoints.end());
	    float minmon6 = *std::min_element(mon6DataPoints.begin(),mon6DataPoints.end());
	    errorMon6 = (maxmon6 - minmon6) / (2 * std::sqrt(count6));
	    errorMon6Vec.push_back(errorMon6);
	    mon6DataPoints.clear();

	    float maxnorm6 = *std::max_element(norm6DataPoints.begin(),norm6DataPoints.end());
	    float minnorm6 = *std::min_element(norm6DataPoints.begin(),norm6DataPoints.end());
	    errorNorm6 = (maxnorm6 - minnorm6) / (2 * std::sqrt(count6));
	    errorNorm6Vec.push_back(errorNorm6);
	    norm6DataPoints.clear();
	    
	    // Sort out average vectors
	    monQ6Vec.push_back(averageMon6);
	    normQ6Vec.push_back(averageNorm6);
	    date6Vec.push_back(date.Convert());
	    averageMon6 = 0;
	    averageNorm6 = 0;
	    count6 = 0;
	  }
	  else {
	    count6++;
	    mon6DataPoints.push_back(monQ.at(i));
	    averageMon6 += monQ.at(i);
	    
	    norm6DataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm6 += totQ.at(i)/monQ.at(i);
	  }
	
	}
      
      }
    
    }
    date6VecVec.push_back(date6Vec);
    date24VecVec.push_back(date24Vec);
    dateVecVec.push_back(dateVec);

    monQVecVec.push_back(monQVec);
    errorMonVecVec.push_back(errorMonVec);

    monQ24VecVec.push_back(monQ24Vec);
    errorMon24VecVec.push_back(errorMon24Vec);
    
    monQ6VecVec.push_back(monQ6Vec);
    errorMon6VecVec.push_back(errorMon6Vec);

    normQVecVec.push_back(normQVec);
    errorNormVecVec.push_back(errorNormVec);

    normQ24VecVec.push_back(normQ24Vec);
    errorNorm24VecVec.push_back(errorNorm24Vec);
    
    normQ6VecVec.push_back(normQ6Vec);
    errorNorm6VecVec.push_back(errorNorm6Vec);


    
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
  format_plots(monQ1, monQ2, monQ3, monQ4, monQ5,leg7, 2870, 2970, "%m-%d","Mon Q",tempNameMon+"All time");
  std::string printname7 = outname + "_monQ_all.png";
  c7.Print(printname7.c_str());


  TCanvas c8("c8","");
  c8.SetGrid();
  TLegend *leg8 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *mon24Q1 = new TGraphErrors(date24VecVec[0].size(), &date24VecVec[0][0], &monQ24VecVec[0][0], 0, &errorMon24VecVec[0][0]);
  TGraphErrors *mon24Q2 = new TGraphErrors(date24VecVec[1].size(), &date24VecVec[1][0], &monQ24VecVec[1][0], 0, &errorMon24VecVec[1][0]);
  TGraphErrors *mon24Q3 = new TGraphErrors(date24VecVec[2].size(), &date24VecVec[2][0], &monQ24VecVec[2][0], 0, &errorMon24VecVec[2][0]);
  TGraphErrors *mon24Q4 = new TGraphErrors(date24VecVec[3].size(), &date24VecVec[3][0], &monQ24VecVec[3][0], 0, &errorMon24VecVec[3][0]);
  TGraphErrors *mon24Q5 = new TGraphErrors(date24VecVec[4].size(), &date24VecVec[4][0], &monQ24VecVec[4][0], 0, &errorMon24VecVec[4][0]);
  format_plots(mon24Q1, mon24Q2, mon24Q3, mon24Q4, mon24Q5,leg8, 2870, 2970, "%H:%M","Mon Q",tempNameMon+"24 Hours");
  std::string printname8 = outname + "_monQ_day.png";
  c8.Print(printname8.c_str());
  
  
  TCanvas c9("c9","");
  c9.SetGrid();
  TLegend *leg9 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *mon6Q1 = new TGraphErrors(date6VecVec[0].size(), &date6VecVec[0][0], &monQ6VecVec[0][0], 0, &errorMon6VecVec[0][0]);
  TGraphErrors *mon6Q2 = new TGraphErrors(date6VecVec[1].size(), &date6VecVec[1][0], &monQ6VecVec[1][0], 0, &errorMon6VecVec[1][0]);
  TGraphErrors *mon6Q3 = new TGraphErrors(date6VecVec[2].size(), &date6VecVec[2][0], &monQ6VecVec[2][0], 0, &errorMon6VecVec[2][0]);
  TGraphErrors *mon6Q4 = new TGraphErrors(date6VecVec[3].size(), &date6VecVec[3][0], &monQ6VecVec[3][0], 0, &errorMon6VecVec[3][0]);
  TGraphErrors *mon6Q5 = new TGraphErrors(date6VecVec[4].size(), &date6VecVec[4][0], &monQ6VecVec[4][0], 0, &errorMon6VecVec[4][0]);
  format_plots(mon6Q1, mon6Q2, mon6Q3, mon6Q4, mon6Q5,leg9, 2870, 2970, "%H:%M","Mon Q",tempNameMon+"6 Hours");
  std::string printname9 = outname + "_monQ_six.png";
  c9.Print(printname9.c_str());
  
  TCanvas c1("c1","");
  c1.SetGrid();
  TLegend *leg1 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *normQ1 = new TGraphErrors(dateVecVec[0].size(), &dateVecVec[0][0], &normQVecVec[0][0], 0, &errorNormVecVec[0][0]);
  TGraphErrors *normQ2 = new TGraphErrors(dateVecVec[1].size(), &dateVecVec[1][0], &normQVecVec[1][0], 0, &errorNormVecVec[1][0]);
  TGraphErrors *normQ3 = new TGraphErrors(dateVecVec[2].size(), &dateVecVec[2][0], &normQVecVec[2][0], 0, &errorNormVecVec[2][0]);
  TGraphErrors *normQ4 = new TGraphErrors(dateVecVec[3].size(), &dateVecVec[3][0], &normQVecVec[3][0], 0, &errorNormVecVec[3][0]);
  TGraphErrors *normQ5 = new TGraphErrors(dateVecVec[4].size(), &dateVecVec[4][0], &normQVecVec[4][0], 0, &errorNormVecVec[4][0]);
  format_plots(normQ1, normQ2, normQ3, normQ4, normQ5,leg1, 0, 0.3, "%m-%d","Norm Q",tempNameNorm+"All time");
  std::string printname1 = outname + "_normQ_all.png";
  c1.Print(printname1.c_str());

  TCanvas c2("c2","");
  c2.SetGrid();
  TLegend *leg2 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *norm24Q1 = new TGraphErrors(date24VecVec[0].size(), &date24VecVec[0][0], &normQ24VecVec[0][0], 0, &errorNorm24VecVec[0][0]);
  TGraphErrors *norm24Q2 = new TGraphErrors(date24VecVec[1].size(), &date24VecVec[1][0], &normQ24VecVec[1][0], 0, &errorNorm24VecVec[1][0]);
  TGraphErrors *norm24Q3 = new TGraphErrors(date24VecVec[2].size(), &date24VecVec[2][0], &normQ24VecVec[2][0], 0, &errorNorm24VecVec[2][0]);
  TGraphErrors *norm24Q4 = new TGraphErrors(date24VecVec[3].size(), &date24VecVec[3][0], &normQ24VecVec[3][0], 0, &errorNorm24VecVec[3][0]);
  TGraphErrors *norm24Q5 = new TGraphErrors(date24VecVec[4].size(), &date24VecVec[4][0], &normQ24VecVec[4][0], 0, &errorNorm24VecVec[4][0]);
  format_plots(norm24Q1, norm24Q2, norm24Q3, norm24Q4, norm24Q5,leg2, 0, 0.3, "%H:%M","Norm Q",tempNameNorm+"24 Hours");
  std::string printname2 = outname + "_normQ_day.png";
  c2.Print(printname2.c_str());
  
  
  TCanvas c3("c3","");
  c3.SetGrid();
  TLegend *leg3 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *norm6Q1 = new TGraphErrors(date6VecVec[0].size(), &date6VecVec[0][0], &normQ6VecVec[0][0], 0, &errorNorm6VecVec[0][0]);
  TGraphErrors *norm6Q2 = new TGraphErrors(date6VecVec[1].size(), &date6VecVec[1][0], &normQ6VecVec[1][0], 0, &errorNorm6VecVec[1][0]);
  TGraphErrors *norm6Q3 = new TGraphErrors(date6VecVec[2].size(), &date6VecVec[2][0], &normQ6VecVec[2][0], 0, &errorNorm6VecVec[2][0]);
  TGraphErrors *norm6Q4 = new TGraphErrors(date6VecVec[3].size(), &date6VecVec[3][0], &normQ6VecVec[3][0], 0, &errorNorm6VecVec[3][0]);
  TGraphErrors *norm6Q5 = new TGraphErrors(date6VecVec[4].size(), &date6VecVec[4][0], &normQ6VecVec[4][0], 0, &errorNorm6VecVec[4][0]);
  format_plots(norm6Q1, norm6Q2, norm6Q3, norm6Q4, norm6Q5,leg3, 0, 0.3, "%H:%M","Norm Q",tempNameNorm+"6 Hours");
  std::string printname3 = outname + "_normQ_six.png";
  c3.Print(printname3.c_str());

  
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
