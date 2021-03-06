#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stol;

Process::Process(int pid) : pid_(pid){
user_ = LinuxParser::User(Pid());
command_ = LinuxParser::Command(Pid());
cpuutilization_ = LinuxParser::CpuUtilization(Pid());
}
// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() 
{
  cpuutilization_ = LinuxParser::CpuUtilization(Pid());
  return cpuutilization_; 
}

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const 
{ 
  return a.cpuutilization_ < this->cpuutilization_; 
}