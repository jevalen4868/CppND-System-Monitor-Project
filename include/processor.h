#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Processor {
 public:
  Processor(float user, float nice, float system, float idle, float iowait,
            float irq, float softirq, float steal, float guest,
            float guest_nice);
  void Calculate(float user, float nice, float system, float idle, float iowait,
                 float irq, float softirq, float steal, float guest,
                 float guest_nice);
  float Utilization() const;  // TODO: See src/processor.cpp
                              // TODO: Declare any necessary private members
 private:
  bool initialized{false};
  float cpu_utilization_{0.0};
  float user_, nice_, system_, idle_, iowait_, irq_, softirq_, steal_, guest_,
      guest_nice_;
};

#endif