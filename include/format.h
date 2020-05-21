#ifndef FORMAT_H
#define FORMAT_H

#include <string>

#define DATE_LENGTH 50
#define RAM_LENGTH 20

namespace Format {
std::string ElapsedTime(long times);
std::string ElapsedTimeWithDays(long seconds);
std::string KBs_to_MBs(long kilobytes);

};                                    // namespace Format

#endif