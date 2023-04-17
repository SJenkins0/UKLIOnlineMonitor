#ifndef TIME_H
#define TIME_H

#include <string>
#include <vector>

int timeCut(std::string inj, bool isCol, bool isLow);
float tof(std::vector<float> injXYZ, std::vector<float> pmtXYZ);
float tofCut(std::string inj, bool isCol);

#endif
