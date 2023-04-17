#ifndef POSITION_H
#define POSITION_H

#include <string>

float injector(std::string inj);
float distance(float pmt, float injector);
int timeCut(std::string inj, bool isCol, bool isLow);

#endif
