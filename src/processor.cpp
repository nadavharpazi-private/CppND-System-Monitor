#include "processor.h"

#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>

#include "linux_parser.h"

using std::string;
using std::vector;
using std::cout;

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {

  // get current cpu utilization values
  vector<long> cpu_utilization = LinuxParser::CpuUtilization();
  long current_total_time = cpu_utilization.back();
  cpu_utilization.pop_back();
  long current_active_time = cpu_utilization.back();

  // find the delta of cpu utilization values against previous round
  long delta_active = current_active_time - prev_active_time;
  long delta_total = current_total_time - prev_total_time;

  // if delta values are zero, use the saved usage from previous round
  if (delta_total == 0 || delta_active == 0) {
    return prev_usage;
  }

  float usage = delta_active * 1.0 / delta_total;

  // save the cpu utilization values to be used in the next round
  prev_usage = usage;
  prev_active_time = current_active_time;
  prev_total_time = current_total_time;
  return usage;
}