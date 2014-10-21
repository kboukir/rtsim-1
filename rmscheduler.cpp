#include "rmscheduler.h"

#include <algorithm>

RMScheduler::RMScheduler(const std::string &filename, unsigned int switch_percent_time)
: AbstractScheduler(filename, switch_percent_time)
{
}

int RMScheduler::schedule()
{
    // Find the schedulable task with the smallest period
    auto t = std::min_element(
        _tasks.begin(),
        _tasks.end(),
        [=] (const Task &a, const Task &b) {
            if (!isTaskSchedulable(a)) {
                // A is finished, sort it after b
                return false;
            } else if (!isTaskSchedulable(b)) {
                // B is finished, sort it after A
                return true;
            } else {
                // Both can be executed, prefer the one with the smallest period
                return a.period < b.period;
            }
        }
    );

    if (!isTaskSchedulable(*t)) {
        // The "best" task is still not schedulable, so schedule no task
        return -1;
    } else {
        // t is the N-th element of _tasks, find this N
        return std::distance(_tasks.begin(), t);
    }
}
