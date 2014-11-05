#include "taskfilegenerator.h"
#include "abstractscheduler.h"

#include <fstream>
#include <algorithm>

static int random_number(int low, int high)
{
    return low + rand() % (high - low + 1);
}

void generateTaskFile(const std::string &filename, int number_tasks, int target_use_percent)
{
    // Choose random periods and offsets for the N tasks
    AbstractScheduler::Task tasks[number_tasks];

    for (int i=0; i<number_tasks; ++i) {
        AbstractScheduler::Task &task = tasks[i];

        task.offset = random_number(0, 100);
        task.period = random_number(10, 200);
    }

    // The target total utilization must be split across the tasks. Considering
    // a line of length target_use_percent, randomly put tasks-1 marks on it so
    // that it is split in tasks parts or random size but having a total length
    // of 100.
    int uses[number_tasks + 1];

    uses[0] = 0;
    uses[number_tasks] = target_use_percent;

    for (int i=1; i<number_tasks; ++i) {
        uses[i] = random_number(1, target_use_percent - 1);
    }

    std::sort(&uses[1], &uses[number_tasks]);

    // Use "uses" to assign to each task its utilization factor
    int delta_use_percent = 0;        // Rounding errors will make tasks having a real utilization factor different from what they should have
    std::ofstream out(filename);

    for (int i=0; i<number_tasks; ++i) {
        AbstractScheduler::Task &task = tasks[i];
        int use_percent = (uses[i+1] - uses[i]) + delta_use_percent;

        task.execution_time = task.period * use_percent / 100;
        task.deadline = random_number(task.execution_time, task.period);

        int real_use_percent = task.execution_time * 100 / task.period;
        delta_use_percent = use_percent - real_use_percent;

        out << task.offset << ' ' << task.period << ' ' << task.deadline << ' ' << task.execution_time << std::endl;
    }

    out.close();
}