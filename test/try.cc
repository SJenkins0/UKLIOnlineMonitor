#include <iostream>
#include "position.h"


int main(int argc, const char *argv[]){


  std::cout << "Enter injector: ";
  std::string inj;
  std::cin >> inj;
  bool col;
  std::cout << "Collimator? ";
  std::cin >> col;
  bool low;
  std::cout << "Low limit? ";
  std::cin >> low;

  std::cout << timeCut(inj, col, low) << std::endl;
  
  return 0;
}
