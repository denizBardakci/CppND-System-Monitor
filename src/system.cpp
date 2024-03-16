#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    processes_.clear();
    set<int> extend_pids;
    // Collect unique process IDs from existing processes
    for (Process& process : processes_) {
        extend_pids.insert(process.Pid());
    }
    // Get all current process IDs
    vector<int> pids = LinuxParser::Pids();
    // Add new processes to the list
    for (int pid : pids) {
        if (extend_pids.find(pid) == extend_pids.end()) {
            processes_.emplace_back(pid);
        }
    }

    // Update CPU utilization for all processes
    for (auto& process : processes_) {
        process.CpuUtilization();
    }

    // Sort processes by CPU utilization in descending order
    std::sort(processes_.rbegin(), processes_.rend());

    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() const { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() const { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() const { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() const {  return LinuxParser::TotalProcesses();  }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() const { return LinuxParser::UpTime(); }
