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
          filestream.close();
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, temp , kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> temp >> kernel;
    stream.close();
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
      stream.close();
      
    }
  }
    return (MemTotal - MemFree)/ MemTotal; 
  }

// Done: Read and return the system uptime
long LinuxParser::UpTime() 
{
  long uptime;
  string line, key;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> key;
    uptime = stol(key);
    stream.close();
  }
  return uptime;
}


// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }
// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long active_jiffies;
  vector<CPUStates> active_states = {
    kUser_,
    kNice_,
    kSystem_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_
  };
  vector<string> utilization = CpuUtilization();
  for (auto state: active_states) {
    active_jiffies += stol(utilization[state]);
  }
  return active_jiffies;
}
// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long idle_jiffies;
  vector<CPUStates> idle_states = {
    kIdle_,
    kIOwait_,
  };
  vector<string> utilization = CpuUtilization();
  for (auto state: idle_states) {
    idle_jiffies += stol(utilization[state]);
  }
  return idle_jiffies;
}
// DONE: Read and return CPU utilization
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
      file.close();
    }
    }
  }
  }
  return Utilization; 
}

// DONE: Read and return the total number of processes
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
        file.close();
      }
  }
  }
  return total; 
}

// DONE: Read and return the number of running processes
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
        file.close();
      }
  }
  }
  return running; 
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line; 
  string command;
  std::ifstream file(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(file.is_open()){
  std::getline(file, command);
    file.close();
  }
  return command;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key;
  string ram = "N/A";
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
      ram = to_string(ramsize);
      file.close();
  } 
  }
  }
  return ram; 
}

// DONE: Read and return the user ID associated with a process

string LinuxParser::Uid(int pid) {
  string line, uid, key;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatusFilename);
  if (file.is_open()){
  while(std::getline(file,line)){
  std::istringstream linestream(line);
    linestream >> key;
    if(key == "Uid:"){
    linestream >> uid;
      file.close();
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
// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string result, user, line, passwd, currentuid;
  string uid = LinuxParser::Uid(pid);
  std::ifstream file(kPasswordPath);
  if(uid.length() && file.is_open()){
  while(std::getline(file,line)){
    std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      while (stream >> user >> passwd >> currentuid) {
        if (currentuid == uid)
          result = user;
      }
    }
  }
  return result;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) 
{
  string line,key;
  string processTime;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  if(file.is_open()){
  std::getline(file,line);
  std::istringstream linestream(line);
    
	int j = 0;
	while(j <22){
  	linestream >> processTime;
 	j++;
}
    file.close();
}
return stol(processTime)/sysconf(_SC_CLK_TCK); 
}