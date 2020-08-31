#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_ = 1,
  kSystem_ = 2,
  kIdle_ = 3,
  kIOwait_ = 4,
  kIRQ_ = 5,
  kSoftIRQ_ = 6,
  kSteal_ = 7,
  kGuest_ = 8,
  kGuestNice_ = 9
};
std::vector<std::vector<std::string>> CpuUtilization();

enum CPUStatesProcess {
  pPid_ = 0,
  pComm_ = 1,
  pState_ = 2,
  pPpid_ = 3,
  pPgrp_ = 4,
  pSession_ = 5,
  pTtyNr_ = 6,
  pTpgId_ = 7,
  pFlags_ = 8,
  pMinflt_ = 9,
  pCminflt_ = 10,
  pMajflt_ = 11,
  pCmajflt_ = 12,
  pUTime_ = 13,
  pSTime_ = 14,
  pCUTime_ = 15,
  pCSTime_ = 16,
  pPriority_ = 17,
  pNice_ = 18,
  pNumThreads_ = 19,
  pItrealvalue_ = 20,
  pStartTime_ = 21
};
std::vector<std::string> CpuUtilization(int pid);

long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
std::vector<std::string> ProcStats(int pid);

};  // namespace LinuxParser

#endif