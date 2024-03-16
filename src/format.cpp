#include <string>
#include <string>

#include <iomanip>
#include <sstream>
#include <chrono>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
std::string Format::ElapsedTime(long s) {
    std::chrono::seconds seconds{s};

    // return std::chrono::format("%T", seconds); // in C++20 :-)

    auto hours = std::chrono::duration_cast<std::chrono::hours>(seconds);
    seconds -= hours;

    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds);
    seconds -= minutes;

    std::stringstream ss{};
    ss << std::setfill('0') << std::setw(2) << hours.count() << ':'  // HH
       << std::setw(2) << minutes.count() << ':'                    // MM
       << std::setw(2) << seconds.count();                          // SS

    return ss.str();
}
