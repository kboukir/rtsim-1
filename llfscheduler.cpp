#include "llfscheduler.h"

LLFScheduler::LLFScheduler(const std::string &filename, unsigned int switch_percent_time)
: AbstractScheduler(filename, switch_percent_time)
{
}

bool LLFScheduler::hasTaskPriority(const AbstractScheduler::Task &a, const AbstractScheduler::Task &b)
{
    return laxity(a) < laxity(b);
}

int LLFScheduler::laxity(const AbstractScheduler::Task& t)
{
    return t.time_to_deadline - (t.execution_time - t.consumed_cycles);
}