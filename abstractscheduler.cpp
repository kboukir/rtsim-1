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

        _tasks.push_back(task);
    }
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

bool AbstractScheduler::schedule(unsigned int time_duration, AbstractLogger *logger)
{
    for (_current_time = 0; _current_time < time_duration; ++_current_time) {
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
            _current_time += switching_time;
        }

        // Switch the current task
        _last_task_scheduled = new_task;

        // Notify the logger of the new task being executed
        logger->notifyTask(this, new_task);
    }

    return true;
}
