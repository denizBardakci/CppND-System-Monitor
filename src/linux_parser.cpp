#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
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
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    double mem_total = 0.0;
    double mem_free = 0.0;

    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (!stream.is_open()) {
        return 0.0f; // Return 0.0 if stream couldn't be opened
    }

    std::string line;
    std::string key;
    double value = 0.0;

    while (std::getline(stream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);

        if (linestream >> key >> value) {
            if (key == "MemTotal") {
                mem_total = value;
            } else if (key == "MemFree") {
                mem_free = value;
            }
        }
    }

    return (mem_total - mem_free) / mem_total;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
    std::ifstream filestream(kProcDirectory + kUptimeFilename);
    if (!filestream.is_open()) {
        return 0; // Return 0 if filestream couldn't be opened
    }

    std::string line;
    int uptime = 0;

    if (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        linestream >> uptime;
    }
    return uptime;
}

// TODO: Read and return the number of jiffies for the system
//long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 1; }

// TODO: Read and return the number of active jiffies for the system
//long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
//long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
std::vector<std::string> LinuxParser::CpuUtilization() {
  string value;
  string line;
  string cpu, user, nice, system, idle;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle;
  }
  return std::vector<string>{user, nice, system, idle};

}
// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    int total_processes = 0;

    std::ifstream stream(kProcDirectory + kStatFilename);
    if (!stream.is_open()) {
        return total_processes; // Return 0 if stream couldn't be opened
    }

    std::string line;
    std::string key;
    int value = 0;

    while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
            if (key == "processes") {
                return value;
            }
        }
    }
    return total_processes;
}

int LinuxParser::RunningProcesses() {
    int running_processes = 0;

    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (!filestream.is_open()) {
        return running_processes; // Return 0 if filestream couldn't be opened
    }

    std::string line;
    std::string key;
    int value = 0;

    while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
            if (key == "procs_running") {
                return value;
            }
        }
    }

    return running_processes;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        return value;
      }
    }
  }
  return value;
}
// TODO: Read and return the memory used by a process
std::string LinuxParser::Ram(int pid) {
    std::string line;
    std::string key;
    std::string size;
    std::string value;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);

    if (!filestream.is_open()) {
        return ""; // Return an empty string if filestream couldn't be opened
    }

    while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value >> size) {
            if (key == "VmSize:" && size == "kB") {
                long temp = std::stol(value) / 1000;
                return std::to_string(temp);
            }
        }
    }
    return ""; // Return an empty string if "VmSize:" is not found or size is not "kB"
}

// TODO: Read and return the user ID associated with a process
std::string LinuxParser::Uid(int pid) {
    std::string line;
    std::string key;
    std::string value;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);

    if (!filestream.is_open()) {
        return ""; // Return an empty string if filestream couldn't be opened
    }

    while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          // e.g. value is root
            if (key == "Uid:") {
                return value;
            }
        }
    }
    return ""; // Return an empty string if "Uid:" is not found
}
// TODO: Read and return the user associated with a process
std::string LinuxParser::User(int pid) {
    std::ifstream filestream(kPasswordPath);
    if (!filestream.is_open()) {
        return ""; // Return an empty string if filestream couldn't be opened
    }

    std::string line;
    std::string username;
    std::string value;
    std::string process_uid = LinuxParser::Uid(pid);

    while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        if (linestream >> username >> value >> value) {
            if (value == process_uid) {
                return username;
            }
        }
    }
    return ""; // Return an empty string if the username is not found
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
   // get the current UpTime of the system, I used LinuxParser::Uptime() of the system functions
    const long systemUptime = UpTime();
    string value;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        string line;
        while (getline(stream, line)) {
            std::istringstream lineStream(line);
            for (uint i = 0; i < 22 && lineStream >> value; ++i) {}
        }
        stream.close();
    }
    return systemUptime - stol(value) / sysconf(_SC_CLK_TCK);
}
