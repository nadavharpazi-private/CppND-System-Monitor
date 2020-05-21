#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  int rounds{0};
  long prev_active_time{0};
  long prev_total_time{0};
  float prev_usage{0.0};
};

#endif