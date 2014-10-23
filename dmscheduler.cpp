#include "dmscheduler.h"

DMScheduler::DMScheduler(const std::string &filename, unsigned int switch_percent_time)
: AbstractScheduler(filename, switch_percent_time)
{
}

bool DMScheduler::hasTaskPriority(const AbstractScheduler::Task& a, const AbstractScheduler::Task& b)
{
    return a.deadline < b.deadline;
}
