#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string time;
  long hours = seconds / 3600;
  seconds = seconds % 3600;
  long minutes = seconds / 60;
  seconds = seconds % 60;

  string hour_str{hours < 10 ? "0" + to_string(hours) : to_string(hours)};
  string minute_str{minutes < 10 ? "0" + to_string(minutes)
                                 : to_string(minutes)};
  string second_str{seconds < 10 ? "0" + to_string(seconds)
                                 : to_string(seconds)};
  time = hour_str + ":" + minute_str + ":" + second_str;

  return time;
}