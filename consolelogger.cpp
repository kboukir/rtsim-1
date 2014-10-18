#include "consolelogger.h"
#include "abstractscheduler.h"

#include <iostream>

void ConsoleLogger::notifyTask(AbstractScheduler* scheduler, int task)
{
    printTime(scheduler->currentTime());

    // Draw the bars
    for (unsigned int i=0; i<scheduler->taskCount(); ++i)
    {
        printTaskPrefix(scheduler, i, scheduler->currentTime());

        if ((int)i == task) {
            // This task is active
            std::cout << "\033[47m \033[0m";
        } else {
            std::cout << ' ';
        }

        std::cout << ' ';
    }

    std::cout << std::endl;
}

void ConsoleLogger::notifySwitch(AbstractScheduler* scheduler, unsigned int duration, unsigned int task)
{
    for (unsigned int t=0; t<duration; ++t) {
        printTime(scheduler->currentTime() + t);

        // Draw A pattern showing that two jobs are being swapped ▒
        for (unsigned int i=0; i<scheduler->taskCount(); ++i)
        {
            printTaskPrefix(scheduler, i, scheduler->currentTime() + t);

            if (i == task || i == (unsigned int)scheduler->currentTask()) {
                std::cout << "\033[43m \033[0m ";
            } else {
                std::cout << "  ";
            }
        }

        std::cout << std::endl;
    }
}

void ConsoleLogger::printTime(unsigned int time)
{
    std::cout << "[";
    std::cout.width(4);
    std::cout << time;
    std::cout.width();
    std::cout << "] ";
}

void ConsoleLogger::printTaskPrefix(AbstractScheduler *scheduler, unsigned int task, unsigned int time)
{
    const AbstractScheduler::Task &t = scheduler->task(task);
    unsigned int time_after_release_time = (time - t.offset) % t.period;

    if (time_after_release_time == 0) {
        // This task has been released
        std::cout << "\033[44m \033[0m";
    } else if (time_after_release_time == t.deadline - 1) {
        // Deadline of this task
        std::cout << "\033[41m \033[0m";
    } else {
        std::cout << ' ';
    }
}
