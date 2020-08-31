#include "process.h"

#include <linux_parser.h>
#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// CREDIT:
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599

int Process::Pid() { return pid_; }

void Process::Pid(int p) { pid_ = p; }

float Process::CpuUtilization() { return cpu_utilization_; }

void Process::CpuUtilization(const std::vector<string> &cpu_utilization,
                             long uptime) {
  float utime{stof(cpu_utilization[0])};
  float stime{stof(cpu_utilization[1])};
  long starttime{stol(cpu_utilization[2])};

  float total_time = utime + stime;

  long hertz = sysconf(_SC_CLK_TCK);
  long seconds = uptime - (starttime / hertz);
  float cpu_usage = (total_time / hertz) / seconds;
  cpu_utilization_ = cpu_usage;
}

string Process::Command() { return cmdline_; }

void Process::Command(std::string cmdline) { cmdline_ = cmdline; }

string Process::Ram() { return ram_; }

void Process::Ram(std::string ram) { ram_ = ram; }

string Process::User() { return user_; }

void Process::User(std::string user) { user_ = user; }

long int Process::UpTime() { return uptime_; }

void Process::UpTime(long int uptime) { uptime_ = uptime; }
