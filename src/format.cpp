#include <string>
#include<iomanip>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int HH = seconds/3600;
  int SS = seconds%3600;
  int MM = SS / 60;
  SS = SS % 60;
  std::ostringstream time;
  string hh = (HH < 10 ? "0" : "") + std::to_string(HH);
    string mm = (MM < 10 ? "0" : "") + std::to_string(MM);
    string ss = (SS < 10 ? "0" : "") + std::to_string(SS);
    return hh + ":" + mm + ":" + ss;
}