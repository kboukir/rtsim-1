#include "taskfilegenerator.h"


#include <fstream>
#include <algorithm>

TaskFileGenerator::TaskFileGenerator(int number_tasks, int target_use_percent)
:_number_tasks(number_tasks), _target_use_percent(target_use_percent), 
_tasks(new AbstractScheduler::Task[number_tasks])
{
    for (int i=0; i<_number_tasks; ++i) {
        AbstractScheduler::Task &task = _tasks[i];

        task.offset = random_number(0, 100);
        task.period = random_number(10, 100);
    }

    // The target total utilization must be split across the tasks. Considering
    // a line of length target_use_percent, randomly put tasks-1 marks on it so
    // that it is split in tasks parts or random size but having a total length
    // of 100.
    int uses[_number_tasks + 1];

    uses[0] = 0;
    uses[_number_tasks] = target_use_percent;

    for (int i=1; i<_number_tasks; ++i) {
        uses[i] = random_number(1, target_use_percent - 1);
    }

    std::sort(&uses[1], &uses[_number_tasks]);

    // Use "uses" to assign to each task its utilization factor
    int delta_use_percent = 0;        // Rounding errors will make tasks having a real utilization factor different from what they should have

    for (int i=0; i<_number_tasks; ++i) {
        AbstractScheduler::Task &task = _tasks[i];
        int use_percent = (uses[i+1] - uses[i]) + delta_use_percent;

        task.execution_time = std::max(1U, task.period * use_percent / 100);
        task.deadline = random_number(task.execution_time, task.period);

        int real_use_percent = task.execution_time * 100 / task.period;
        delta_use_percent = use_percent - real_use_percent;

    }

}


int TaskFileGenerator::random_number(int low, int high)
{
    return low + rand() % (high - low + 1);
}

void TaskFileGenerator::generateTaskFile(const std::string& filename)
{
    std::ofstream out(filename);

    for (int i=0; i<_number_tasks; ++i) {
        AbstractScheduler::Task &task = _tasks[i];
        out << task.offset << ' ' << task.period << ' ' << task.deadline << ' ' << task.execution_time << std::endl;
    }

    out.close();
}

int TaskFileGenerator::getNumberTasks()
{
    return _number_tasks;
}

int TaskFileGenerator::getTargetUsePercent()
{
    return _target_use_percent;
}

AbstractScheduler::Task* TaskFileGenerator::getTasks()
{
    return _tasks;
}
