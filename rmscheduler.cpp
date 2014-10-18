#include "rmscheduler.h"

RMScheduler::RMScheduler(const std::string &filename, unsigned int switch_percent_time)
: AbstractScheduler(filename, switch_percent_time)
{
    // The next release of each task is its period + its offset
    TaskInfo info;

    for (unsigned int i=0; i<taskCount(); ++i) {
        const Task &t = task(i);

        info.executed_cycles = 0;
        info.next_release_absolute = t.offset + t.period;

        _task_infos.push_back(info);
    }
}

int RMScheduler::schedule()
{
    int best_task = -1;
    unsigned int best_task_period = 0;

    for (std::size_t i=0; i<_task_infos.size(); ++i) {
        TaskInfo &info = _task_infos[i];
        const Task &t = task(i);

        if (info.next_release_absolute == currentTime()) {
            // An job is released by a task
            info.executed_cycles = 0;
            info.next_release_absolute += t.period;
        }

        // Check if this task has a period smaller than the current best one
        if (info.executed_cycles < t.execution_time &&
            (best_task == -1 || t.period < best_task_period)) {
            best_task = i;
            best_task_period = t.period;
        }
    }

    if (best_task != -1) {
        // We execute a cycle of the task
        _task_infos[best_task].executed_cycles++;
    }

    return best_task;
}
  