#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() const { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
    long sys_uptime = LinuxParser::UpTime();

    std::ifstream stream(LinuxParser::kProcDirectory +  std::to_string(pid) + LinuxParser::kStatFilename);
    if (!stream.is_open()) {
        return 0.0; // Return 0.0 if the stream couldn't be opened
    }

    std::string line;
    std::vector<std::string> stat;

    if (std::getline(stream, line)) {
        std::istringstream linestream(line);
        stat.assign(std::istream_iterator<std::string>(linestream), std::istream_iterator<std::string>());
    }

    if (stat.size() < 22) {
        return 0.0; // Return 0.0 if the required information is not present
    }

    long utime = std::stol(stat[13]);
    long stime = std::stol(stat[14]);
    long cutime = std::stol(stat[15]);
    long cstime = std::stol(stat[16]);
    long starttime = std::stol(stat[21]);
    long total_time = utime + stime + cutime + cstime;
    long seconds = sys_uptime - (starttime / sysconf(_SC_CLK_TCK));

    return static_cast<float>(total_time) / sysconf(_SC_CLK_TCK) / seconds;
}

// Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(pid); }

// Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid); }

// Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(pid); }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& other) const { 
    return CpuUtilization() < other.CpuUtilization(); 
}