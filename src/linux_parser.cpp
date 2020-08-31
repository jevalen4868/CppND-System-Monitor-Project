#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>

using std::getline;
using std::ifstream;
using std::istringstream;
using std::stof;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream line_stream(line);
      while (line_stream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version_text, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);
    line_stream >> os >> version_text >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  namespace filesystem = std::experimental::filesystem;
  for (const filesystem::directory_entry &entry :
       filesystem::directory_iterator(kProcDirectory)) {
    if (filesystem::is_directory(entry)) {
      string filename = entry.path().filename();
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string desc_text, total, free, line;
  ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      istringstream line_stream(line);
      line_stream >> desc_text;
      if (desc_text == "MemTotal:") {
        line_stream >> total;
      }
      if (desc_text == "MemFree:") {
        line_stream >> free;
        // done, no more parsing.
        break;
      }
    }
  }
  float totalf = stof(total);
  return (totalf - stof(free)) / totalf;
}

long LinuxParser::UpTime() {
  string uptime_sec, idle_sec, line;
  ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream line_stream(line);
    line_stream >> uptime_sec >> idle_sec;
  }
  return std::stol(uptime_sec);
}

vector<vector<string>> LinuxParser::CpuUtilization() {
  vector<vector<string>> cpu_util_vec{};
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice, line;
  ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      istringstream line_stream(line);
      line_stream >> cpu;
      if (cpu.substr(0, 3) == "cpu") {
        line_stream >> user >> nice >> system >> idle >> iowait >> irq >>
            softirq >> steal >> guest >> guest_nice;
        // should correspond with index of states enum in linux parser.
        vector<string> cpu_utilization{user,   nice,      system,  idle,
                                       iowait, irq,       softirq, steal,
                                       guest,  guest_nice};
        cpu_util_vec.push_back(cpu_utilization);
      } else {
        break;
      }
    }
  }

  return cpu_util_vec;
}

vector<string> LinuxParser::CpuUtilization(int pid) {
  string utime, stime, cutime, cstime, starttime;

  vector<string> proc_stats{ProcStats(pid)};
  utime = proc_stats[CPUStatesProcess::pUTime_];
  stime = proc_stats[CPUStatesProcess::pSTime_];
  starttime = proc_stats[CPUStatesProcess::pStartTime_];

  vector<string> cpu_utilization{utime, stime, starttime};

  return cpu_utilization;
}

int LinuxParser::TotalProcesses() {
  string line, name, value;
  ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      istringstream line_stream(line);
      line_stream >> name >> value;
      if (name == "processes") {
        return stoi(value);
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() {
  string line, name, value;
  ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      istringstream line_stream(line);
      line_stream >> name >> value;
      if (name == "procs_running") {
        return stoi(value);
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid) {
  string cmd;
  ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    getline(stream, cmd);
    return cmd;
  }
  return string();
}

string LinuxParser::Ram(int pid) {
  string desc_text, vm_size, line;
  ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      istringstream line_stream(line);
      line_stream >> desc_text;
      if (desc_text == "VmSize:") {
        line_stream >> vm_size;
        return to_string(stoi(vm_size) / 1000) + " MB";
      }
    }
  }
  return string();
}

string LinuxParser::Uid(int pid) {
  string desc_text, uid, line;
  ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      istringstream line_stream(line);
      line_stream >> desc_text;
      if (desc_text == "Uid:") {
        line_stream >> uid;
        return uid;
      }
    }
  }
  return string{};
}

string LinuxParser::User(int pid) {
  string uid{Uid(pid)}, user;
  string passwd_line;
  ifstream stream_passwd(kPasswordPath);
  if (stream_passwd.is_open()) {
    while (getline(stream_passwd, passwd_line)) {
      stringstream line_stream_passwd{passwd_line};
      string user_name;
      string token;
      // user name
      getline(line_stream_passwd, user_name, ':');
      // group
      getline(line_stream_passwd, token, ':');
      // need the third token, which is the uid
      getline(line_stream_passwd, token, ':');
      if (strcmp(token.c_str(), uid.c_str()) == 0) {
        return user_name;
      }
    }
  }
  return string();
}

long LinuxParser::UpTime(int pid) {
  vector<string> proc_stats{ProcStats(pid)};
  long time_started{stol(proc_stats[CPUStatesProcess::pStartTime_]) /
                    sysconf(_SC_CLK_TCK)};
  long uptime{UpTime()};
  return uptime - time_started;
}

vector<string> LinuxParser::ProcStats(int pid) {
  vector<string> proc_stats{};
  string token, line;
  ifstream stream(kProcDirectory + to_string(pid) + "/" + kStatFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream line_stream(line);
    while (line_stream.good()) {
      getline(line_stream, token, ' ');
      proc_stats.push_back(token);
    }
  }
  return proc_stats;
}
