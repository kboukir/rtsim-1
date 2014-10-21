#include "abstractscheduler.h"
#include "abstractlogger.h"

#include <fstream>
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
    nextTick();
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

void AbstractScheduler::nextTick()
{
    _current_time++;

    for (std::size_t i=0; i<_tasks.size(); ++i) {
        Task &t = _tasks[i];

        if (t.time_to_deadline > 0) {
            // Don't let time_do_deadline go below zero
            t.time_to_deadline--;
        }

        if (--t.time_to_release <= 0) {
            // New release
            t.consumed_cycles = 0;
            t.time_to_release = t.period;
            t.time_to_deadline = t.deadline;
        }
    }
}

bool AbstractScheduler::schedule(unsigned int time_duration, AbstractLogger *logger)
{
    for (_current_time = 0; _current_time < time_duration; nextTick()) {
        // Ask the subclass which task must be scheduled now
        int new_task = schedule();
        unsigned int switching_time = 0;

        if (new_task != _last_task_scheduled && _switch_percent_time != 0) {
            // Task switch, so some time is lost in the context switch
            switching_time = (
                task(_last_task_scheduled).execution_time + task(new_task).execution_time
            ) * _switch_percent_time / 100;
        }

        // Notify the logger of the possible switching time
        if (switching_time > 0 && _last_task_scheduled != -1 && new_task != -1) {
            logger->notifySwitch(this, switching_time, new_task);

            // Advance the current time
            for (unsigned int i=0; i<switching_time; ++i) {
                nextTick();
            }
        }

        // Switch the current task
        _last_task_scheduled = new_task;

        // Tell the new task that it has consumed a cycle
        _tasks[new_task].consumed_cycles++;

        // Notify the logger of the new task being executed
        logger->notifyTask(this, new_task);
    }

    return true;
}
