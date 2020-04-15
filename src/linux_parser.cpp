#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;
using std::stoi;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key, value;
  //creating variables for Total memory and Free Memory
  float MemTotal, MemFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
    std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "MemTotal:"){MemTotal = stof(value);}
      else if (key == "MemFree:"){MemFree = stof(value);}
      else continue;
      
    }
  }
    return (MemTotal - MemFree)/ MemTotal; 
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{
  string line, key;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream iss;
    iss >> key;
  }
  return stol(key);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{ 
  vector <string> CpuUt = LinuxParser::CpuUtilization();
  long time = 0;
  for(unsigned int i = 0; i < CpuUt.size(); i++){
    if( i == LinuxParser::CPUStates::kGuest_ || i == LinuxParser::CPUStates::kGuestNice_){
    time -= stol(CpuUt[i]);
    }
    else
      time += stol(CpuUt[i]);
  }
    
  return time; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
  long activejiffies;
  string line,key;
  string parsepid, comm, state, ppid, pgrp, session, tty, tpgid, flags, minflt, cminflt, majflt, cmajflt, utime, stime, cutime, cstime;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  if(file.is_open()){
  std::getline(file,line);
  std::istringstream linestream(line);
    linestream >> parsepid >> comm >> state >> ppid >> pgrp >> session >> tty >> flags;
    linestream >> minflt >> cminflt >> majflt >> utime >> stime >> cutime >> cstime;
    activejiffies = stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
  }
  return activejiffies; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{
  vector<string> cpu = LinuxParser::CpuUtilization();
  return stol(cpu[LinuxParser::CPUStates::kIdle_]) + stol(cpu[LinuxParser::CPUStates::kIOwait_]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{
  string line, key, value;
  vector <string> Utilization;
  std::ifstream file(kProcDirectory + kStatFilename);
  if (file.is_open()){
  while(std::getline(file,line)){
  std::istringstream lines(line);
    lines >> key;
    if(key == "cpu"){
    while(lines >> value){
    Utilization.push_back(value);
    }
    }
  }
  }
  return Utilization; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  string line, key, value;
  int total;
  std::ifstream file(kProcDirectory + kStatFilename);
  if (file.is_open()){
    while( std::getline(file,line)){
  std::istringstream linestream(line);
      linestream >> key >> value;
      if( key == "processes"){
      total = stoi(value);
      }
  }
  }
  return total; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{
  string line, key, value;
  int running;
  std::ifstream file(kProcDirectory + kStatFilename);
  if (file.is_open()){
    while( std::getline(file,line)){
  std::istringstream linestream(line);
      linestream >> key >> value;
      if( key == "procs_running"){
      running = stoi(value);
      }
  }
  }
  return running; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line; 
  string command;
  std::ifstream file(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(file.is_open()){
  std::getline(file, command);
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, ram;
  int ramsize;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatusFilename);
  if(file.is_open()){
  while(std::getline(file,line)){
  std::istringstream linestream(line);
    linestream >> key;
    if(key == "VmSize:"){
      linestream >> ram;
      ramsize = stoi(ram);
      ramsize /= 1000;
  } 
  }
  }
  return to_string(ramsize); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, uid, key;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatusFilename);
  if (file.is_open()){
  while(std::getline(file,line)){
  std::istringstream linestream(line);
    linestream >> key;
    if(key == "Uid:"){
    linestream >> uid;
    }
  }
  }
  return uid; 
}
float LinuxParser::CpuUtilization(int pid){
long total_time;
double seconds;
  string line,key;
  string parsepid, comm, state, ppid, pgrp, session, tty, tpgid, flags, minflt, cminflt, majflt, cmajflt, utime, stime, cutime, cstime, nice, num_threads, itrealvalue, starttime;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  if(file.is_open()){
  std::getline(file,line);
  std::istringstream linestream(line);
    linestream >> parsepid >> comm >> state >> ppid >> pgrp >> session >> tty >> flags;
    linestream >> minflt >> cminflt >> majflt >> utime >> stime >> cutime >> cstime >> nice >> num_threads >> itrealvalue >> starttime;
    total_time = stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
    seconds = LinuxParser::UpTime() - (stol(starttime))/sysconf(_SC_CLK_TCK); 
}
  return ((total_time/sysconf(_SC_CLK_TCK))/seconds);
}
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string user,line,passwd,currentUid;
  string uid = LinuxParser::Uid(pid);
  std::ifstream file(kPasswordPath);
  if(uid.length() && file.is_open()){
  while(std::getline(file,line)){
    std::istringstream linestream(line);
    linestream >> user >> passwd >> currentUid;
    if(currentUid == uid){
    return user;
    }
  } 
}
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  long uptime;
  string line,key;
  string parsepid, comm, state, ppid, pgrp, session, tty, tpgid, flags, minflt, cminflt, majflt, cmajflt, utime, stime, cutime, cstime;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  if(file.is_open()){
  std::getline(file,line);
  std::istringstream linestream(line);
    linestream >> parsepid >> comm >> state >> ppid >> pgrp >> session >> tty >> flags;
    linestream >> minflt >> cminflt >> majflt >> utime >> stime >> cutime >> cstime;
    uptime = stol(stime)/ sysconf(_SC_CLK_TCK);
    
  }
 return uptime; 
}