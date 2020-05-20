#ifndef FORMAT_H
#define FORMAT_H

#include <string>

#define DATE_LENGTH 200

namespace Format {
std::string ElapsedTime(long times);
std::string ElapsedTimeWithDays(long seconds);
};                                    // namespace Format

#endif