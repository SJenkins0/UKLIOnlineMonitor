/* ---------------- time.cc ---------------- *
 * Contains the time cuts for the monitoring *
 * system. Also contains the ToF definition. *
 * Could be used for other codes requiring   *
 * timing cuts.                              *
 *                                           *
 * (Compiled using g++ 4.8.5 with flag       *
 *                           -std=c++11)     *
 *                                           *
 * SOFTWARE REQUIRED:                        *
 * ---> ROOT                                 *
 *                                           *
 *                jmmcelwee1@sheffield.ac.uk *
 * ------------------------- J. McElwee ---- */

#include <string>
#include <iostream>
#include <math.h>
#include <vector>


float tof(std::vector<float> injXYZ, std::vector<float> pmtXYZ){

  float c = 30/1.2; // speed of light in water (cm/ns)

  float x = injXYZ[0] - pmtXYZ[0];
  float y = injXYZ[1] - pmtXYZ[1];
  float z = injXYZ[2] - pmtXYZ[2];
  float dist = sqrt(x*x + y*y + z*z);

  return dist/c;

}

float tofCut(std::string inj, bool isCol){
  
  int tof = 0;

  if (isCol){
    if (inj == "B1") tof = 1190;
    else if (inj == "B2") tof = 1240;
    else if (inj == "B3") tof = 1250;
    else if (inj == "B4") tof = 1225;
    else if (inj == "B5") tof = 1250;
    else {
      std::cout << "\033[1;31m[ERROR]\033[0m Incorrect injector postion. Please enter B1-5." << std::endl;
      exit(0);
    }
  }
  else {
    tof = 1400;
  }
  
  return tof;
}





int timeCut(std::string inj, bool isCol, bool isLow){

  int low [2] = {};
  int high [2] = {};
  
  if (inj == "B1"){
    low[0] = 1230;
    high[0] = 1380;
    
    low[1] = 1150;
    high[1] = 1450;
    
  }
  else if (inj == "B2"){
    low[0] = 1240;
    high[0] = 1400;
    
    low[1] = 1210;
    high[1] = 1410;
    
  }
  else if (inj == "B3"){
    low[0] = 1240;
    high[0] = 1410;
    
    low[1] = 1200;
    high[1] = 1360;
    
  }
  else if (inj == "B4"){
    low[0] = 1230;
    high[0] = 1380;
    
    low[1] = 1190;
    high[1] = 1370;
    
  }
  else if (inj == "B5"){
    low[0] = 1240;
    high[0] = 1400;
    
    low[1] = 1140;
    high[1] = 1410;
    
  }
  
  if (isLow){
    if (isCol){
      return low[0];
    }
    else if (!isCol){
      return low[1];
    }
  }
  else if (!isLow){
    if (isCol){
      return high[0];
    }
    else if (!isCol){
      return high[1];
    }
  }
}
