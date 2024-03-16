#include "processor.h"
#include "linux_parser.h"

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    vector<string> use = LinuxParser::CpuUtilization();

    float user = stof(use[0]);
    float nice = stof(use[1]);
    float system = stof(use[2]);
    float idle = stof(use[3]);    
    float total = user + nice + system + idle;
    return (total - idle) / total;

}