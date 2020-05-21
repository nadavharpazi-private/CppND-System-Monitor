#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  user_ = LinuxParser::User(pid_);
  command_ = LinuxParser::Command(pid_);
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
  return a.CpuUtilization() < this->CpuUtilization(); // sort from highest to lowest cpu usage
//  return pid_ < a.Pid(); // sort by pid, from smallest to largest number
//  return a.RamRSS() < this->RamRSS(); // sort by Ram, from high to low
}