#include "rmscheduler.h"

RMScheduler::RMScheduler(const std::string &filename, unsigned int switch_percent_time)
: AbstractScheduler(filename, switch_percent_time)
{
}

bool RMScheduler::hasTaskPriority(const AbstractScheduler::Task& a, const AbstractScheduler::Task& b)
{
    return a.period < b.period;
}
