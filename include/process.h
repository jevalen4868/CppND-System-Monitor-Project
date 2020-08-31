#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();
  void Pid(int p);
  std::string User();
  void User(std::string user);
  std::string Command();
  void Command(std::string cmdline);
  float CpuUtilization();
  void CpuUtilization(const std::vector<std::string>& cpu_utilization,
                      long uptime);
  std::string Ram();
  void Ram(std::string ram);
  long int UpTime();
  void UpTime(long int uptime);
  bool operator<(Process const& a) const;

 private:
  int pid_{};
  std::string user_{};
  long int uptime_{};
  std::string cmdline_{};
  std::string ram_{};
  float cpu_utilization_{};
};

#endif