#include "graph2d.h"
#include "taskfilegenerator.h"
#include "abstractscheduler.h"
#include "edfscheduler.h"
#include "dummylogger.h"
#include <string>
#include <iostream>
#include <list>


static void timePercentage(const char *title, unsigned int time, unsigned int total)
{
    std::cout << title  << (time * 100 / total) << " %" << std::endl;
}

// Compute the cartesian product of the three lists, execute EDF scheduler 
// for each tuple that belongs to the product and procude 6 graphs in gnuplot format 
// (switching_time X idle_time, switching_time X schedulability, number_of_tasks X idle_time, 
// number_of_tasks X schedulability, use_percentage X idle_time and use_percentage X schedulability).
void test(std::list<unsigned int> switch_percent_times, std::list<unsigned int> number_of_tasks, std::list<unsigned int> utilisations) {
    std::list<Graph2d::Point> y_idle_x_switching_time;
    std::list<Graph2d::Point> y_schedulable_x_switching_time;
    std::list<Graph2d::Point> y_idle_x_tasks;
    std::list<Graph2d::Point> y_schedulable_x_tasks;
    std::list<Graph2d::Point> y_idle_x_use;
    std::list<Graph2d::Point> y_schedulable_x_use;
    
    unsigned int simulation_index = 1;
    
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
                unsigned int idle_time_percent = (scheduler.currentTime() - scheduler.activeTime() - scheduler.switchingTime()) * 100 / scheduler.currentTime();
                Graph2d::Point pt;
                
                pt.x = *it_percent;
                pt.y = idle_time_percent;
                y_idle_x_switching_time.push_back(pt);
                
                pt.y = scheduler.isSystemSchedulable() ? 1 : 0;
                y_schedulable_x_switching_time.push_back(pt);
                
                pt.x = *it_task;
                pt.y = idle_time_percent;
                y_idle_x_tasks.push_back(pt);
                
                pt.y = scheduler.isSystemSchedulable() ? 1 : 0;
                y_schedulable_x_tasks.push_back(pt);
                
                pt.x = *it_u;
                pt.y = idle_time_percent;
                y_idle_x_use.push_back(pt);
                
                pt.y = scheduler.isSystemSchedulable() ? 1 : 0;
                y_schedulable_x_use.push_back(pt);
                
                std::cout << "======Statistics of simulation " << simulation_index << "======" << std::endl
                    << "-> Simulation inputs" << std::endl
                    << "Switching time: " << *it_percent << "%" << std::endl 
                    << "Number of tasks: " << *it_task << std::endl 
                    << "System load: " << *it_u << "%" << std::endl
                    << "-> Simulation outputs" << std::endl
                    << "System schedulable: " << (scheduler.isSystemSchedulable() ? "yes" : "no") << std::endl
                    << "Simulation time: " << scheduler.currentTime() << " time steps" << std::endl;
                timePercentage("Active time: ", scheduler.activeTime(), scheduler.currentTime());
                timePercentage("Switching time: ", scheduler.switchingTime(), scheduler.currentTime());
                timePercentage("Idle time: ", scheduler.currentTime() - scheduler.activeTime() - scheduler.switchingTime(), scheduler.currentTime());
                std::cout << "Context switches: " << scheduler.numberPreemptions() << std::endl
                    << std::endl;
                
                ++simulation_index;
            }
        }
    }
    
    Graph2d graph1(y_idle_x_switching_time, "Switching time (%)", "Idle time (%)");
    graph1.writeInGnuplotFile("y_idle_x_switching_time.gnuplot");
    
    Graph2d graph2(y_schedulable_x_switching_time, "Switching time (%)", "Schedulable");
    graph2.writeInGnuplotFile("y_schedulable_x_switching_time.gnuplot");
    
    Graph2d graph3(y_idle_x_tasks, "Number of tasks", "Idle time (%)");
    graph3.writeInGnuplotFile("y_idle_x_tasks.gnuplot");
    
    Graph2d graph4(y_schedulable_x_tasks, "Number of tasks", "Schedulable");
    graph4.writeInGnuplotFile("y_schedulable_x_tasks.gnuplot");
    
    Graph2d graph5(y_idle_x_use, "Utilisation (%)", "Idle time (%)");
    graph5.writeInGnuplotFile("y_idle_x_use.gnuplot");
    
    Graph2d graph6(y_schedulable_x_use, "Utilisation (%)", "Schedulable");
    graph6.writeInGnuplotFile("y_schedulable_x_use.gnuplot");
}


int main(int argc, char **argv) {
    
    std::list<unsigned int> switch_percent_times;
    switch_percent_times.push_back(5);
    
    std::list<unsigned int> number_of_tasks;
    number_of_tasks.push_back(2);
    number_of_tasks.push_back(3);
    
    std::list<unsigned int> utilisations;
    utilisations.push_back(40);
    utilisations.push_back(70);
    utilisations.push_back(90);
    utilisations.push_back(110);

    
    test(switch_percent_times, number_of_tasks, utilisations);

    return 0;
}
