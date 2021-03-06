#include "abstractscheduler.h"
#include "abstractlogger.h"

#include <fstream>
#include <algorithm>
#include <iostream>

template<typename T>
static T gcd(T a, T b)
{
    for (;;) {
        if (a == 0) return b;
            b %= a;
        if (b == 0) return a;
            a %= b;
    }
}

template<typename T>
T lcm(T a, T b)
{
    T temp = gcd(a, b);

    return temp ? (a / temp * b) : 0;
}

AbstractScheduler::AbstractScheduler(const std::string &filename, unsigned int switch_percent_time)
: _current_time(0),
  _switch_percent_time(switch_percent_time),
  _last_task_scheduled(-1)
{
    // Open and read the description file. Each of its lines contains
    // offset period deadline wcet
    Task task;
    std::ifstream file(filename.c_str());

    task.index = 0;

    while (!file.eof()) {
        task.deadline = 0;

        file >> task.offset;
        file >> task.period;
        file >> task.deadline;
        file >> task.execution_time;

        task.consumed_cycles = 0;
        task.time_to_deadline = task.deadline + task.offset;
        task.time_to_release = task.offset;

        if (task.deadline != 0) {
            _tasks.push_back(task);
            task.index++;
        }
    }

    // Dummy nextTick so that the tasks can start to run
    nextTick(nullptr);
    --_current_time;
}

AbstractScheduler::AbstractScheduler(std::vector<Task> tasks, unsigned int switch_percent_time)
: _tasks(tasks),
  _current_time(0),
  _switch_percent_time(switch_percent_time),
  _last_task_scheduled(-1)
{
    for (unsigned int i=0; i<_tasks.size(); ++i) {
        _tasks[i].index = i;
        _tasks[i].consumed_cycles = 0;
        _tasks[i].time_to_deadline = _tasks[i].deadline + _tasks[i].offset;
        _tasks[i].time_to_release = _tasks[i].offset;
    }
    
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

bool AbstractScheduler::isSystemSchedulable() const
{
    return _system_schedulable;
}

unsigned int AbstractScheduler::activeTime() const
{
    return _active_time;
}

unsigned int AbstractScheduler::switchingTime() const
{
    return _switching_time;
}

unsigned int AbstractScheduler::numberPreemptions() const
{
    return _number_preemptions;
}

unsigned int AbstractScheduler::taskCount() const
{
    return _tasks.size();
}

const AbstractScheduler::Task &AbstractScheduler::task(int index) const
{
    return _tasks[index];
}

unsigned int AbstractScheduler::idealSimulationTime() const
{
    unsigned int period = 1;
    unsigned int max_offset = 0;

    for (std::size_t i=0; i<_tasks.size(); ++i) {
        const Task &t = _tasks[i];

        max_offset = std::max(max_offset, t.offset);
        period = lcm(period, t.period);
    }

    return max_offset + (2 * period);
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

void AbstractScheduler::schedule(unsigned int time_duration, AbstractLogger *logger)
{
    _system_schedulable = true;
    _active_time = 0;
    _switching_time = 0;
    _number_preemptions = 0;

    for (_current_time = 0; _current_time < time_duration; _system_schedulable &= nextTick(logger)) {
        // Ask the subclass which task must be scheduled now
        int new_task = schedule();
        unsigned int switching_time = 0;

        if (new_task != _last_task_scheduled && _switch_percent_time != 0 && new_task != -1) {
            unsigned int wcets_sum = 0;
            if (_last_task_scheduled != -1) {
                // Task switch with preemption of another job, so some time is lost to store 
                // previous context and load the new one
                wcets_sum = task(_last_task_scheduled).execution_time 
                    + task(new_task).execution_time;

                _number_preemptions++;
            }
            else {
                // Task switch without preemption, time is lost only to load the new context
                wcets_sum = task(new_task).execution_time;
            }
            switching_time = ceil((
                    wcets_sum
                ) * _switch_percent_time / 100);
        }

        // Notify the logger of the possible switching time
        if (switching_time > 0) {
            logger->notifySwitch(this, switching_time, new_task);

            // Advance the current time
            for (unsigned int i=0; i<switching_time; ++i) {
                _system_schedulable &= nextTick(logger);
                _switching_time++;
            }
        }

        // Switch the current task
        _last_task_scheduled = new_task;

        // Tell the new task that it has consumed a cycle
        if (new_task != -1) {
            _tasks[new_task].consumed_cycles++;
            _active_time++;
        }

        // Notify the logger of the new task being executed
        logger->notifyTask(this, new_task);
    }
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
