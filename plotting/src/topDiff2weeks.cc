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

void extract_data(std::ifstream &file, std::vector<double> &totQ,std::vector<double> &barrelQ,std::vector<int> &month,std::vector<int> &day,std::vector<int> &hour,std::vector<int> &minute,std::vector<int> &second,std::vector<double> &bottomQ,std::vector<double> &bz0y0Q);
void format_plots(TGraph *graph1,TLegend *leg ,float min, float max, std::string timeform, std::string yaxis, std::string title);
void format_plots_multi(TGraph *graph1, TGraph *graph2, TLegend *leg,float min, float max, std::string timeform, std::string yaxis, std::string title);

int main(int argc, char *argv[]){

  
  int oNameSwitch = 0;
  std::string ldir = "/disk02/usr6/jmcelwee/monitoringData/plotting";
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


  std::string fileEx = ldir + "/top_diffuser.dat";
  std::cout << fileEx << std::endl;

  
  time_t timer;
  time(&timer);

  gStyle->SetCanvasColor(0);
  gStyle->SetTitleColor(0);
  gStyle->SetOptStat(0);  

  //  std::vector<std::vector<float>> dateVecVec, totQVecVec, spotQVecVec, date6VecVec,totQ6VecVec,spotQ6VecVec, date24VecVec,totQ24VecVec,spotQ24VecVec,errorTotVecVec,errorTot6VecVec,errorTot24VecVec,errorSpotVecVec,errorSpot6VecVec,errorSpot24VecVec;


  std::vector<int> run, subrun, year, month, day, hour, minute, second;
  std::vector<double> totQ, barrelQ, bottomQ, bz0y0Q;
  std::ifstream file(fileEx);
  extract_data(file,totQ,barrelQ,month,day,hour,minute,second,bottomQ,bz0y0Q);


  
  int avenum = 1500;
  std::vector<float> totDataPoints, totQVec, ratioDataPoints, ratioQVec, dateVec, errorRatioVec, errorTotVec, bbDataPoints, bbQVec, errorBBVec;
  float averageTot = 0;
  float averageRatio = 0;
  float averageBB = 0;
  int count = 0;


  
  //int time6 = timer - 2*2997000;
  int time6 = timer - 3*604800;
  std::vector<float> barrelDataPoints, barrelVec, errorBarVec;
  std::vector<float> bottomDataPoints, bottomVec, errorBotVec;
  std::vector<float> bzyDataPoints, bzyVec, errorBzyVec;
  float averageBot = 0;
  float averageBar = 0;
  float averageBzy = 0;
  float errorTot, errorRatio, errorBB;
  float errorBar, errorBot, errorBzy;
  
  for (int i=0; i<totQ.size(); i++){
    TDatime date(2020,month.at(i),day.at(i),hour.at(i),minute.at(i),second.at(i));
    if (date.Convert() > time6 && totQ.at(i) > 100000 && totQ.at(i) < 165000){
      if (i == 0){
	count++;
	averageTot += totQ.at(i);
	averageRatio += bottomQ.at(i)/barrelQ.at(i);
	averageBB += bottomQ.at(i)/bz0y0Q.at(i);

	averageBot += bottomQ.at(i)/totQ.at(i);
	averageBar += barrelQ.at(i)/totQ.at(i);
	averageBzy += bz0y0Q.at(i)/totQ.at(i);
	bottomDataPoints.push_back(bottomQ.at(i)/totQ.at(i));
	barrelDataPoints.push_back(barrelQ.at(i)/totQ.at(i));
	bzyDataPoints.push_back(bz0y0Q.at(i)/totQ.at(i));

      
	if (bottomQ.at(i)/bz0y0Q.at(i) > 1.0 && bottomQ.at(i)/bz0y0Q.at(i) < 1.7){
	  bbDataPoints.push_back(bottomQ.at(i)/bz0y0Q.at(i));
	}
	totDataPoints.push_back(totQ.at(i));
	ratioDataPoints.push_back(bottomQ.at(i)/barrelQ.at(i));
      }
      else if (i % avenum == 0){
	count++;
	// Average Calculation
	averageTot += totQ.at(i);
	averageTot /= count;
	averageRatio += bottomQ.at(i)/barrelQ.at(i);
	averageRatio /= count;
	averageBB += bottomQ.at(i)/bz0y0Q.at(i);
	averageBB /= count;

	averageBot += bottomQ.at(i)/totQ.at(i);
	averageBar += barrelQ.at(i)/totQ.at(i);
	averageBzy += bz0y0Q.at(i)/totQ.at(i);
	averageBot /= count;
	averageBar /= count;
	averageBzy /= count;

	totDataPoints.push_back(totQ.at(i));
	ratioDataPoints.push_back(bottomQ.at(i)/barrelQ.at(i));
	if (bottomQ.at(i)/bz0y0Q.at(i) > 1.0 && bottomQ.at(i)/bz0y0Q.at(i) < 1.7){
	  bbDataPoints.push_back(bottomQ.at(i)/bz0y0Q.at(i));
	}
	bottomDataPoints.push_back(bottomQ.at(i)/totQ.at(i));
	barrelDataPoints.push_back(barrelQ.at(i)/totQ.at(i));
	bzyDataPoints.push_back(bz0y0Q.at(i)/totQ.at(i));
      

	float max = *std::max_element(totDataPoints.begin(),totDataPoints.end());
	float min = *std::min_element(totDataPoints.begin(),totDataPoints.end());
	errorTot = (max - min) / (2 * std::sqrt(count));
	//std::cout << max << " " << min << std::endl;
	float maxsp = *std::max_element(ratioDataPoints.begin(),ratioDataPoints.end());
	float minsp = *std::min_element(ratioDataPoints.begin(),ratioDataPoints.end());
	errorRatio = (maxsp - minsp) / (2 * std::sqrt(count));
	float maxbb = *std::max_element(bbDataPoints.begin(),bbDataPoints.end());
	float minbb = *std::min_element(bbDataPoints.begin(),bbDataPoints.end());
	errorBB = (maxbb - minbb) / (2 * std::sqrt(count));
	//      if (errorBB > 0.05) std::cout << errorBB << std::endl;
	float maxba = *std::max_element(barrelDataPoints.begin(),barrelDataPoints.end());
	float minba = *std::min_element(barrelDataPoints.begin(),barrelDataPoints.end());
	errorBar = (maxba - minba) / (2 * std::sqrt(count));
	float maxbo = *std::max_element(bottomDataPoints.begin(),bottomDataPoints.end());
	float minbo = *std::min_element(bottomDataPoints.begin(),bottomDataPoints.end());
	errorBot = (maxbo - minbo) / (2 * std::sqrt(count));
	float maxbzy = *std::max_element(bzyDataPoints.begin(),bzyDataPoints.end());
	float minbzy = *std::min_element(bzyDataPoints.begin(),bzyDataPoints.end());
	errorBzy = (maxbzy - minbzy) / (2 * std::sqrt(count));

      
	errorTotVec.push_back(errorTot);
	errorRatioVec.push_back(errorRatio);
	errorBBVec.push_back(errorBB);
	errorBotVec.push_back(errorBot);
	errorBarVec.push_back(errorBar);
	errorBzyVec.push_back(errorBzy);
	totDataPoints.clear();
	ratioDataPoints.clear();
	bbDataPoints.clear();
	bottomDataPoints.clear();
	barrelDataPoints.clear();
	bzyDataPoints.clear();
      

      
	// Sort out average vectors
	totQVec.push_back(averageTot);
	ratioQVec.push_back(averageRatio);
	bbQVec.push_back(averageBB);
	dateVec.push_back(date.Convert());
	bottomVec.push_back(averageBot);
	barrelVec.push_back(averageBar);
	bzyVec.push_back(averageBzy);
	averageTot = 0;
	averageRatio = 0;
	averageBB = 0;
	averageBot = 0;
	averageBar = 0;
	averageBzy = 0;
	count = 0;
      }
      else {
	averageTot += totQ.at(i);
	averageRatio += bottomQ.at(i)/barrelQ.at(i);
	averageBB += bottomQ.at(i)/bz0y0Q.at(i);
	count++;

	averageBot += bottomQ.at(i)/totQ.at(i);
	averageBar += barrelQ.at(i)/totQ.at(i);
	averageBzy += bz0y0Q.at(i)/totQ.at(i);

	totDataPoints.push_back(totQ.at(i));
	ratioDataPoints.push_back(bottomQ.at(i)/barrelQ.at(i));
	if (bottomQ.at(i)/bz0y0Q.at(i) > 1.0 && bottomQ.at(i)/bz0y0Q.at(i) < 1.7){
	  bbDataPoints.push_back(bottomQ.at(i)/bz0y0Q.at(i));
	}
	bottomDataPoints.push_back(bottomQ.at(i)/totQ.at(i));
	barrelDataPoints.push_back(barrelQ.at(i)/totQ.at(i));
	bzyDataPoints.push_back(bz0y0Q.at(i)/totQ.at(i));
      }
      
    }
  }

  
   
  if (oNameSwitch == 0) outname = "/home/jmcelwee/monitoring/plotting/top_diffuser";
  std::string tempNameTot = "Total Q (top diffuser)";
  std::string tempNameRatio = "Charge Ratio (top diffuser) ";
  std::string tempNameBB = "Charge Ratio (top diffuser - z&y > 0) ";
  std::string tempNameBot = "Normalised Bottom Q ";
  std::string tempNameBar = "Normalised Barrel Q ";
  std::string tempNameBzy = "Normalised Barrel Q (z&y > 0) ";
  
  TCanvas c1("c1","", 850, 500);
  c1.SetGrid();
  TLegend *leg1 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *totQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &totQVec[0], 0, &errorTotVec[0]);
  format_plots(totQ1, leg1, 140000,160000, "%m-%d","Tot Q (p.e)",tempNameTot+"3 weeks");
  std::string printname1 = outname + "_totQ_2weeks.png";
  c1.Print(printname1.c_str());

  
  TCanvas c2("c2","", 850, 500);
  c2.SetGrid();
  TLegend *leg2 = new TLegend(0.75,0.8,0.9,0.9);
  TGraphErrors *ratioQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &ratioQVec[0], 0, &errorRatioVec[0]);
  format_plots(ratioQ1, leg2, 0.3, 0.6,"%m-%d","bottomQ/barrelQ",tempNameRatio+"3 weeks");
  std::string printname2 = outname + "_ratioQ_2weeks.png";
  c2.Print(printname2.c_str());

  TCanvas c7("c7","", 850, 500);
  c7.SetGrid();
  TLegend *leg7 = new TLegend(0.75,0.8,0.9,0.9);
  TGraphErrors *bbQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bbQVec[0], 0, &errorBBVec[0]);
  format_plots(bbQ1, leg7, 1.37, 1.55,"%m-%d","bottomQ/barrelQ",tempNameBB+"3 weeks");
  std::string printname7 = outname + "_bbQ_2weeks.png";
  c7.Print(printname7.c_str());


  TCanvas c10("c10","", 850, 500);
  c10.SetGrid();
  TLegend *leg10 = new TLegend(0.75,0.8,0.9,0.9);
  TGraphErrors *botQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bottomVec[0], 0, &errorBotVec[0]);
  format_plots(botQ1, leg10, 1.37, 1.55,"%m-%d","bottomQ/totQ",tempNameBot+"3 weeks");
  std::string printname10 = outname + "_botQ_2weeks.png";
  c10.Print(printname10.c_str());

  TCanvas c11("c11","", 850, 500);
  c11.SetGrid();
  TLegend *leg11 = new TLegend(0.75,0.8,0.9,0.9);
  TGraphErrors *barQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &barrelVec[0], 0, &errorBarVec[0]);
  format_plots(barQ1, leg11, 1.37, 1.55,"%m-%d","barrelQ/totQ",tempNameBar+"3 weeks");
  std::string printname11 = outname + "_barQ_2weeks.png";
  c11.Print(printname11.c_str());

  TCanvas c12("c12","", 850, 500);
  c12.SetGrid();
  TLegend *leg12 = new TLegend(0.75,0.8,0.9,0.9);
  TGraphErrors *bzyQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bzyVec[0], 0, &errorBzyVec[0]);
  format_plots(bzyQ1, leg12, 1.37, 1.55,"%m-%d","bz0y0Q/totQ",tempNameBzy+"3 weeks");
  std::string printname12 = outname + "_bzyQ_2weeks.png";
  c12.Print(printname12.c_str());

  return 0;
  
}




void extract_data(std::ifstream &file,std::vector<double> &totQ, std::vector<double> &barrelQ,std::vector<int> &month,std::vector<int> &day,std::vector<int> &hour,std::vector<int> &minute,std::vector<int> &second,std::vector<double> &bottomQ,std::vector<double> &bz0y0Q){

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
    totQ.push_back(std::stod(entries.at(8)));
    barrelQ.push_back(std::stod(entries.at(9)));
    bz0y0Q.push_back(std::stod(entries.at(10)));
    bottomQ.push_back(std::stod(entries.back()));
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
