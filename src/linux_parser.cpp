#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

static const auto Hertz = sysconf(_SC_CLK_TCK);

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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key, value;
  string line;
  long memTotal = 0, memFree = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = stoi(value);
        } else if (key == "MemFree:") {
          memFree = stoi(value);
        }
        if (memFree != 0 && memTotal != 0) {
          float memUsed = (memTotal - memFree) * 1.0 / memTotal;
          return memUsed;
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime = 0;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// DONE: Read and return the number of active jiffies for a PID
float LinuxParser::CpuUtilization(int pid) {
  string line, dummy;
  long utime = 0, stime = 0, active_time = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + "/" + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 13; i++) {
      linestream >> dummy;
    }
    linestream >> utime >> stime;
    active_time = utime + stime;
  }

  float seconds = UpTime(pid);
  if (seconds == 0) {
    return 0.0;
  }
  float cpu_usage = (active_time / Hertz) / seconds;
  return cpu_usage;
}

// CPU jiffies are read from /proc/stat according to this order:
// "cpu"         (0) (ignored, marked "dummy")
// user;         (1)
// nice;         (2)
// system; 	     (3)
// idle;   	     (4)
// iowait; 	     (5)
// irq;    	     (6)
// softirq;	     (7)
// steal;  	     (8)
// guest;  	     (9)
// guest_nice;   (10)

// DONE: Read and return the number of jiffies for the system
vector <long> LinuxParser::Jiffies() {
  string dummy;
  long user = 0, nice = 0, system = 0, idle = 0, iowait = 0;
  long irq = 0, softirq = 0, steal = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> dummy >> user >> nice >> system >> idle >> iowait;
    linestream >> irq >> softirq >> steal;  // skip guest & guest_nice
  }

  vector<long> jiffies;
  jiffies.push_back(steal);
  jiffies.push_back(softirq);
  jiffies.push_back(irq);
  jiffies.push_back(iowait);
  jiffies.push_back(idle);
  jiffies.push_back(system);
  jiffies.push_back(nice);
  jiffies.push_back(user);
  return jiffies;
}

// DONE: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {

  vector <long>  jiffies = Jiffies();
  long user =    jiffies.back(); jiffies.pop_back();
  long nice =    jiffies.back(); jiffies.pop_back();
  long system =  jiffies.back(); jiffies.pop_back();
  long idle =    jiffies.back(); jiffies.pop_back();
  long iowait =  jiffies.back(); jiffies.pop_back();
  long irq =     jiffies.back(); jiffies.pop_back();
  long softirq = jiffies.back(); jiffies.pop_back();
  long steal =   jiffies.back(); jiffies.pop_back();

  // calculate idle time, active time and tital time (in jiffies)
  long idle_time = idle + iowait;
  long active_time = user + nice + system + irq + softirq + steal;
  long total_time = active_time + idle_time;

  vector<long> result;
  result.push_back(active_time);
  result.push_back(total_time);
  return result;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key, value;
  string line;
  int total_processes = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          total_processes = stoi(value);
          return total_processes;
        }
      }
    }
  }
  return total_processes;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key, value;
  string line;
  int running_processes = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          running_processes = stoi(value);
          return running_processes;
        }
      }
    }
  }
  return running_processes;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + "/" +
                       kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command;
}

// DONE: Read and return the memory used by a process (in KB)
long LinuxParser::RamRSS(int pid) {
  string key, value;
  string line;
  long ram = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + "/" + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmRSS:") {
          ram = stol(value);
          return ram;
        }
      }
    }
  }
  return ram;
}

// DONE: Read and return the memory used by a process (in KB)
long LinuxParser::RamVirtual(int pid) {
  string key, value;
  string line;
  long ram = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + "/" + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          ram = stol(value);
          return ram;
        }
      }
    }
  }
  return ram;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string key, value;
  string line;
  string uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + "/" + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
          return uid;
        }
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string key;
  string value;
  string justx;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> justx >> key) {
        if (key == uid) {
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: Read and return the uptime of a process
long int LinuxParser::UpTime(int pid) {
  string dummy;
  string line;
  long start_time = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + "/" + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; i++) {
      linestream >> dummy;
    }
    linestream >> start_time;
  }
  long process_uptime = UpTime() - (start_time / Hertz);
  return process_uptime;
}
