#include "graph2d.h"
#include "taskfilegenerator.h"
#include "abstractscheduler.h"
#include "edfscheduler.h"
#include "dummylogger.h"
#include <string>
#include <iostream>
#include <list>


void test(std::list<unsigned int> switch_percent_times, std::list<unsigned int> number_of_tasks, std::list<unsigned int> utilisations) {
    std::list<Graph2d::Point> y_idle_x_switching_time;
    std::list<Graph2d::Point> y_idle_x_tasks;
    std::list<Graph2d::Point> y_idle_x_use;
    for(std::list<unsigned int>::iterator it_task = number_of_tasks.begin(); it_task!=number_of_tasks.end(); ++it_task)
    {
        for(std::list<unsigned int>::iterator it_u = utilisations.begin(); it_u!=utilisations.end(); ++it_u)
        {
            TaskFileGenerator taskFileGenerator(*it_task, *it_u);
            unsigned int number_of_tasks = taskFileGenerator.getNumberTasks();
            AbstractScheduler::Task *tasks = taskFileGenerator.getTasks();
            std::vector<AbstractScheduler::Task> task_vector(tasks, tasks+number_of_tasks);
            

            for(std::list<unsigned int>::iterator it_percent = switch_percent_times.begin(); it_percent!=switch_percent_times.end(); ++it_percent)
            {
                EDFScheduler scheduler = EDFScheduler(task_vector, *it_percent);
                AbstractLogger *logger = new DummyLogger;
                scheduler.schedule(scheduler.idealSimulationTime(), logger);
                Graph2d::Point pt;
                pt.x = *it_percent;
                pt.y = (scheduler.currentTime() - scheduler.activeTime() - scheduler.switchingTime()) * 100 / scheduler.currentTime();
                y_idle_x_switching_time.push_back(pt);
                pt.x = *it_task;
                y_idle_x_tasks.push_back(pt);
                pt.x = *it_u;
                y_idle_x_use.push_back(pt);
            }
        }
    }
    
    Graph2d graph1(y_idle_x_switching_time, "Switching time (%)", "Idle time (%)");
    graph1.writeInGnuplotFile("y_idle_x_switching_time.gnuplot");
    Graph2d graph2(y_idle_x_tasks, "Number of tasks", "Idle time (%)");
    graph2.writeInGnuplotFile("y_idle_x_tasks.gnuplot");
    Graph2d graph3(y_idle_x_use, "Utilisation (%)", "Idle time (%)");
    graph3.writeInGnuplotFile("y_idle_x_use.gnuplot");
}


int main(int argc, char **argv) {
    
    std::list<unsigned int> switch_percent_times;
    switch_percent_times.push_back(5);
    
    std::list<unsigned int> number_of_tasks;
    number_of_tasks.push_back(2);
    
    std::list<unsigned int> utilisations;
    utilisations.push_back(40);
    utilisations.push_back(70);
    utilisations.push_back(90);
    utilisations.push_back(110);

    
    test(switch_percent_times, number_of_tasks, utilisations);

    return 0;
}
