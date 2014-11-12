#include "edfscheduler.h"

EDFScheduler::EDFScheduler(const std::string &filename, unsigned int switch_percent_time)
: AbstractScheduler(filename, switch_percent_time)
{
}

bool EDFScheduler::hasTaskPriority(const AbstractScheduler::Task& a, const AbstractScheduler::Task& b)
{
    return a.time_to_deadline < b.time_to_deadline;
}