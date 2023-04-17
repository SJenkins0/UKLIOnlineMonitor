/* -------------- position.cc -------------- *
 * Contains the position calculation for the *
 * monitoring system. Could be used for other*
 * codes requiring injector positions.       *
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
#include "TVector3.h"

float injector(std::string inj){
  
  float position = 0.0;
  if (inj == "B1") position = 1223.9;
  else if (inj == "B2") position = 742.3;
  else if (inj == "B3") position = -200.3;
  else if (inj == "B4") position = -747.1;
  else if (inj == "B5") position = -1413.4;
  else {
    std::cout << "\033[1;31m[ERROR]\033[0m Incorrect injector postion. Please enter B1-5." << std::endl;
    exit(0);
  } 
  return position;
}


TVector3 target(std::string inj){

  TVector3 tar;
  if (inj == "B1") tar.SetXYZ(-1595,-550.6,1223);
  else if (inj == "B2") tar.SetXYZ(-1567,-620,753);
  else if (inj == "B3") tar.SetXYZ(-1575,-553.6,-215);
  else if (inj == "B4") tar.SetXYZ(-1515,-810,-756.2);
  else if (inj == "B5") tar.SetXYZ(-1500, -600, -1413);
  else {
    std::cout << "\033[1;31m[ERROR]\033[0m Incorrect injector postion. Please enter B1-5." << std::endl;
    exit(0);
  }

  return tar;
}
  

/*float injector(std::string inj){
  float position = 0.0;
  if (inj == "B1") position = 1232.5;
  else if (inj == "B2") position = 595.95;
  else if (inj == "B3") position = -40.35;
  else if (inj == "B4") position = -605.95;
  else if (inj == "B5") position = -1242.25;
  else {
    std::cout << "\033[1;31m[ERROR]\033[0m Incorrect injector postion. Please enter B1-5." << std::endl;
    exit(0);
  } 
  return position;


  }*/


float distance(float pmt, float injector){
  
  float dist;
  dist = pmt - injector;
  return dist;
}


