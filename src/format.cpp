#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) {
  int hours = seconds / (60 * 60);
  seconds -= hours * (60 * 60);
  long minutes = seconds / 60;
  seconds -= minutes * 60;
  char formatted[DATE_LENGTH]{};
  sprintf(formatted, "%02d:%02ld:%02ld", hours, minutes, seconds);
  return string(formatted);
}

string Format::ElapsedTimeWithDays(long seconds) {
  int days = seconds / (60 * 60 * 24);
  seconds -= days * (60 * 60 * 24);
  int hours = seconds / (60 * 60);
  seconds -= hours * (60 * 60);
  long minutes = seconds / 60;
  seconds -= minutes * 60;
  char formatted[DATE_LENGTH]{};
  sprintf(formatted, "%d Days, %02d:%02ld:%02ld", days, hours, minutes, seconds);
  return string(formatted);
}