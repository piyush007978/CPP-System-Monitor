#include <unistd.h>
#include<string>
#include<vector>
#include<thread>

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
  long previoustime = LinuxParser::Jiffies();
  long previousidle = LinuxParser::IdleJiffies();
  std::this_thread::sleep_for (std::chrono::seconds(1));
  long totaltime = LinuxParser::Jiffies() - previoustime;
  long idletime = LinuxParser::IdleJiffies() - previousidle;
  return (float)(totaltime - idletime)/(float)(totaltime);
 }