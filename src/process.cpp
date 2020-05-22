#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  // pid, user name and command are not going to change,
  // therefore they are stored as class members by the constructor
  user_ = LinuxParser::User(pid_);
  command_ = LinuxParser::Command(pid_);
  if (pid_ && !user_.empty() && !command_.empty()) {
      // signal that process object is correct and initialized properly
      ok_ = true;
  }
}

int Process::Pid() const { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const {
  return LinuxParser::CpuUtilization(pid_);
}

// DONE: Return the command that generated this process
string Process::Command() const { return command_; }

// DONE: Return this process's memory utilization
long Process::RamRSS() const { return LinuxParser::RamRSS(pid_); }

// DONE: Return this process's memory utilization
long Process::RamVirtual() const { return LinuxParser::RamVirtual(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() const { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return a.CpuUtilization() < this->CpuUtilization(); // sort by cpu usage, from highest to lowest
}