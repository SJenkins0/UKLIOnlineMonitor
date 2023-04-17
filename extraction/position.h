#ifndef POSITION_H
#define POSITION_H

#include <string>
#include <vector>
#include "TVector3.h"

float injector(std::string inj);
float distance(float pmt, float injector);
TVector3 target(std::string inj);

#endif
