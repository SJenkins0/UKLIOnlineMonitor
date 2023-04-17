/* ---------------- rndFunc.cc ---------------- *    
 * Random code required for running the SK      * 
 * monitoring system. This can probably be used * 
 * for other purposes too, as currently it only * 
 * converts char arrays to strings.             * 
 * SOFTWARE REQUIRED:                           *  
 *                                              * 
 *                   jmmcelwee1@sheffield.ac.uk * 
 * ---------------------------- J. McElwee ---- */

#include <string>
#include <iostream>

std::string convertToString(char *arr){

  std::string str = "";
  int arr_size = sizeof(arr)/sizeof(char);

  for (int i=0; i < arr_size; i++){
    str += arr[i];
  }

  return str;

}

void help(){
  std::cout << "\033[1;32m[HELP]\033[37m ----- SK Monitoring -----\033[0m" << std::endl;
  std::cout << "\033[1;32m[HELP]\033[0m Syntax: ./monitor -f <filename> [-i <inj>] [-c/d] [-o <outname>]" << std::endl;
  std::cout << "\033[1;32m[HELP]\033[0m Args:" << std::endl;
  std::cout << "\033[1;32m[HELP]\033[0m <filename>     File to be analysed - required." << std::endl;
  std::cout << "\033[1;32m[HELP]\033[0m [-i <inj>]     Flag for choosing an injector position - argument required. Default is 'B1'." << std::endl;
  std::cout << "\033[1;32m[HELP]\033[0m [-c/d]         Switches for choosing diffuser or collimator. Default is diffuser." << std::endl;
  std::cout << "\033[1;32m[HELP]\033[0m [-o <outname>] Change the outfile name from default - argument required." << std::endl;
  std::cout << "\033[1;32m[HELP]\033[0m [-r <nrun>]    Input run number for saving - argument required." << std::endl;
}
