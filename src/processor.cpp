#include <unistd.h>
#include<string>
#include<vector>
#include<thread>

#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
//https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

//https://knowledge.udacity.com/questions/121171
Processor::Processor() {
   pTotal = LinuxParser::Jiffies();
   pActive = LinuxParser::ActiveJiffies();
}
float Processor::Utilization() {
  long active = LinuxParser::ActiveJiffies();
  long total = LinuxParser::Jiffies();
  float cpu = float(active - pActive) / (total - pTotal);
   pTotal = total; 
   pActive = active;
  return cpu;
}