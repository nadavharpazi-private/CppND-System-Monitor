#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<float> cpu_utilization = LinuxParser::CpuUtilization(); 
    float usage = cpu_utilization.back();
    cpu_utilization.pop_back();
    return usage;
}