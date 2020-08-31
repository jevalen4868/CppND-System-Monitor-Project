#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::stof;
using std::string;
using std::vector;

vector<Processor>& System::Cpu() {
  vector<vector<string>> cpu_util_vec{LinuxParser::CpuUtilization()};
  // if no cpu data provided, construct
  if (cpus_.empty()) {
    for (vector<string> cpu_util : cpu_util_vec) {
      Processor processor{stof(cpu_util[LinuxParser::CPUStates::kUser_]),
                          stof(cpu_util[LinuxParser::CPUStates::kNice_]),
                          stof(cpu_util[LinuxParser::CPUStates::kSystem_]),
                          stof(cpu_util[LinuxParser::CPUStates::kIdle_]),
                          stof(cpu_util[LinuxParser::CPUStates::kIOwait_]),
                          stof(cpu_util[LinuxParser::CPUStates::kIRQ_]),
                          stof(cpu_util[LinuxParser::CPUStates::kSoftIRQ_]),
                          stof(cpu_util[LinuxParser::CPUStates::kSteal_]),
                          stof(cpu_util[LinuxParser::CPUStates::kGuest_]),
                          stof(cpu_util[LinuxParser::CPUStates::kGuestNice_])};
      cpus_.push_back(processor);
    }
  } else {
    for (size_t i{0}; i < cpu_util_vec.size(); i++) {
      Processor* proc_i = &cpus_[i];
      vector<string> cpu_util = cpu_util_vec[i];
      proc_i->Calculate(stof(cpu_util[LinuxParser::CPUStates::kUser_]),
                        stof(cpu_util[LinuxParser::CPUStates::kNice_]),
                        stof(cpu_util[LinuxParser::CPUStates::kSystem_]),
                        stof(cpu_util[LinuxParser::CPUStates::kIdle_]),
                        stof(cpu_util[LinuxParser::CPUStates::kIOwait_]),
                        stof(cpu_util[LinuxParser::CPUStates::kIRQ_]),
                        stof(cpu_util[LinuxParser::CPUStates::kSoftIRQ_]),
                        stof(cpu_util[LinuxParser::CPUStates::kSteal_]),
                        stof(cpu_util[LinuxParser::CPUStates::kGuest_]),
                        stof(cpu_util[LinuxParser::CPUStates::kGuestNice_]));
    };
  }
  return cpus_;
}

vector<Process>& System::Processes() {
  processes_.clear();
  for (int pid : LinuxParser::Pids()) {
    Process process;
    process.Pid(pid);
    process.User(LinuxParser::User(pid));
    process.UpTime(LinuxParser::UpTime(pid));
    process.Command(LinuxParser::Command(pid));
    process.Ram(LinuxParser::Ram(pid));
    process.CpuUtilization(LinuxParser::CpuUtilization(pid),
                           LinuxParser::UpTime());
    processes_.push_back(process);
  }
  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }