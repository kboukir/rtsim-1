#include "fixedscheduler.h"

FixedScheduler::FixedScheduler(const std::string &filename, unsigned int switch_percent_time)
: AbstractScheduler(filename, switch_percent_time)
{
}

bool FixedScheduler::hasTaskPriority(const AbstractScheduler::Task& a, const AbstractScheduler::Task& b)
{
    return a.index < b.index;
}
