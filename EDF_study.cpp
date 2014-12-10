#include "graph2d.h"
#include "taskfilegenerator.h"
#include "abstractscheduler.h"
#include "edfscheduler.h"
#include "dummylogger.h"
#include <string>
#include <iostream>


void test(std::list<int> switch_percent_times, std::list<int> number_of_tasks, std::list<int> utilisations) {
    for(std::list<int>::iterator it_task = number_of_tasks.begin(); it_task!=number_of_tasks.end(); ++it_task)
    {
        for(std::list<int>::iterator it_u = utilisations.begin(); it_u!=utilisations.end(); ++it_u)
        {
            TaskFileGenerator taskFileGenerator(*it_task, *it_u);
            int number_of_tasks = taskFileGenerator.getNumberTasks();
            AbstractScheduler::Task *tasks = taskFileGenerator.getTasks();

            for(std::list<int>::iterator it_percent = switch_percent_times.begin(); it_percent!=switch_percent_times.end(); ++it_percent)
            {
                EDFScheduler scheduler = EDFScheduler(std::vector<AbstractScheduler::Task>(tasks, tasks+sizeof(AbstractScheduler::Task)*number_of_tasks), *it_percent);
                AbstractLogger *logger = new DummyLogger;
                scheduler.schedule(scheduler.idealSimulationTime(), logger);
            }
        }
    }
}


int main(int argc, char **argv) {
    
    std::list<int> switch_percent_times;
    switch_percent_times.push_back(5);
    switch_percent_times.push_back(10);
    
    std::list<int> number_of_tasks;
    number_of_tasks.push_back(2);
    number_of_tasks.push_back(4);
    
    std::list<int> utilisations;
    utilisations.push_back(70);
    utilisations.push_back(90);
    utilisations.push_back(110);
    
    test(switch_percent_times, number_of_tasks, utilisations);

    return 0;
}
