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
    Process(int pid_) : pid(pid_) {}
    int Pid() const;
    std::string User() const;
    std::string Command() const;
    float CpuUtilization() const;
    std::string Ram() const;
    long int UpTime() const;
    bool operator<(Process const& a) const;

private:
    int pid;
};

#endif