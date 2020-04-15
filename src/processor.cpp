#include <unistd.h>
#include<string>
#include<vector>
#include<thread>

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float active = float(LinuxParser::ActiveJiffies());
  float total = float(LinuxParser::Jiffies());
  return active / total;
}