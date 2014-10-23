#include "abstractscheduler.h"
#include "abstractlogger.h"

#include <fstream>
#include <algorithm>
#include <iostream>

AbstractScheduler::AbstractScheduler(const std::string &filename, unsigned int switch_percent_time)
: _current_time(0),
  _switch_percent_time(switch_percent_time),
  _last_task_scheduled(-1)
{
    // Open and read the description file. Each of its lines contains
    // offset period deadline wcet
    Task task;
    std::ifstream file(filename.c_str());

    while (!file.eof()) {
        file >> task.offset;
        file >> task.period;
        file >> task.deadline;
        file >> task.execution_time;

        task.consumed_cycles = 0;
        task.time_to_deadline = task.deadline;
        task.time_to_release = task.offset;

        _tasks.push_back(task);
    }

    // Dummy nextTick so that the tasks can start to run
    nextTick(nullptr);
    --_current_time;
}

AbstractScheduler::~AbstractScheduler()
{
}

int AbstractScheduler::currentTask() const
{
    return _last_task_scheduled;
}

unsigned int AbstractScheduler::currentTime() const
{
    return _current_time;
}

unsigned int AbstractScheduler::taskCount() const
{
    return _tasks.size();
}

const AbstractScheduler::Task &AbstractScheduler::task(int index) const
{
    return _tasks[index];
}

bool AbstractScheduler::isTaskSchedulable(const Task &t) const
{
    return _current_time >= t.offset &&
           t.consumed_cycles < (int)t.execution_time;
}

bool AbstractScheduler::nextTick(AbstractLogger *logger)
{
    bool ok = true;

    _current_time++;

    for (std::size_t i=0; i<_tasks.size(); ++i) {
        Task &t = _tasks[i];

        if (t.time_to_deadline > 0) {
            // Don't let time_do_deadline go below zero
            t.time_to_deadline--;
        } else if (t.consumed_cycles != (int)t.execution_time) {
            // Oh oh, a deadline has been missed
            if (logger) {
                logger->notifyDeadlineMiss(this, i);
            }

            t.consumed_cycles = t.execution_time;   // This is a bit cheating but this avoids notifyDeadlineMiss to be called once for each tick between a deadline and the next release time
            ok = false;
        }

        if (t.time_to_release > 0) {
            t.time_to_release--;
        } else {
            // New release
            t.consumed_cycles = 0;
            t.time_to_release = t.period - 1;
            t.time_to_deadline = t.deadline - 1;
        }
    }

    return ok;
}

bool AbstractScheduler::schedule(unsigned int time_duration, AbstractLogger *logger)
{
    bool ok = true;

    for (_current_time = 0; _current_time < time_duration; ok &= nextTick(logger)) {
        // Ask the subclass which task must be scheduled now
        int new_task = schedule();
        unsigned int switching_time = 0;

        if (new_task != _last_task_scheduled && _switch_percent_time != 0 && _last_task_scheduled != -1 && new_task != -1) {
            // Task switch, so some time is lost in the context switch
            switching_time = (
                task(_last_task_scheduled).execution_time + task(new_task).execution_time
            ) * _switch_percent_time / 100;
        }

        // Notify the logger of the possible switching time
        if (switching_time > 0) {
            logger->notifySwitch(this, switching_time, new_task);

            // Advance the current time
            for (unsigned int i=0; i<switching_time; ++i) {
                ok &= nextTick(logger);
            }
        }

        // Switch the current task
        _last_task_scheduled = new_task;

        // Tell the new task that it has consumed a cycle
        if (new_task != -1) {
            _tasks[new_task].consumed_cycles++;
        }

        // Notify the logger of the new task being executed
        logger->notifyTask(this, new_task);
    }

    return ok;
}

int AbstractScheduler::schedule()
{
    // Find the schedulable task with the greatest priority
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
                // Both can be executed, prefer the one with the greatest priority
                return hasTaskPriority(a, b);
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
