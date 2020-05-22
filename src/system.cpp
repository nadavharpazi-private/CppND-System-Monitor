#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::cout;

// DONE: Return the system's CPU object
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();

  vector<int> Pids = LinuxParser::Pids();
  for (int pid : Pids) {
    Process process(pid);
    // check that the process object has initialized properly.
    if (!process.isOK()) {
      continue;
    }
    // add the process it to the vector only if it OK.
    processes_.push_back(process);
  }

  auto lambda_expression = [](const Process & a, const Process & b) {
    return a < b;
  };
  sort(processes_.begin(), processes_.end(), lambda_expression);
  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE:Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }