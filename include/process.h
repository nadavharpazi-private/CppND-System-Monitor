#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);

  int Pid() const;                               
  std::string User() const;                      
  std::string Command() const;                   
  float CpuUtilization() const;                  // TODO: See src/process.cpp
  std::string Ram() const;                       
  long int UpTime() const;                       
  bool operator<(Process const& a) const;  

  // TODO: Declare any necessary private members
 private:
    int pid_{0};
    std::string user_{""};
    std::string command_{""};
};

#endif