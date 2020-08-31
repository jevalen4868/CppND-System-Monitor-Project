#include "processor.h"

#include <linux_parser.h>

#include <string>
#include <vector>

using std::stol;
using std::string;
using std::vector;

// CREDIT:
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() const { return cpu_utilization_; }

void Processor::Calculate(float user, float nice, float system, float idle,
                          float iowait, float irq, float softirq, float steal,
                          float guest, float guest_nice) {
  // calculate the previous values if initialized
  float prev_idle{0}, prev_nonidle{0}, prev_total{0};
  if (initialized) {
    prev_idle = idle_ + iowait_;
    prev_nonidle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
    prev_total = prev_idle + prev_nonidle;
  }

  // read the new values
  user_ = user;
  nice_ = nice;
  system_ = system;
  idle_ = idle;
  iowait_ = iowait;
  irq_ = irq;
  softirq_ = softirq;
  steal_ = steal;
  guest_ = guest;
  guest_nice_ = guest_nice;

  // we can proceed to calculate cpu usage if initialized.
  if (initialized) {
    float true_idle = idle_ + iowait_;
    float non_idle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
    float total = true_idle + non_idle;
    float total_diff = total - prev_total;
    float idle_diff = true_idle - prev_idle;
    cpu_utilization_ = (total_diff - idle_diff) / total_diff;
  }

  initialized = true;
}

Processor::Processor(float user, float nice, float system, float idle,
                     float iowait, float irq, float softirq, float steal,
                     float guest, float guest_nice)
    : user_{user},
      nice_{nice},
      system_{system},
      idle_{idle},
      iowait_{iowait},
      irq_{irq},
      softirq_{softirq},
      steal_{steal},
      guest_{guest},
      guest_nice_{guest_nice} {}
