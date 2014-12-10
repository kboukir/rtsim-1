#include "graph2d.h"
#include <string>
#include <iostream>


void test(std::list<int> switch_percent_times, std::list<int> number_of_tasks, std::list<int> utilisations) {
    for(std::list<int>::iterator it1 = switch_percent_times.begin(); it1!=switch_percent_times.end(); ++it1)
    {
        for(std::list<int>::iterator it2 = number_of_tasks.begin(); it2!=number_of_tasks.end(); ++it2)
        {
            for(std::list<int>::iterator it3 = utilisations.begin(); it3!=utilisations.end(); ++it3)
            {
                
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

    return 0;
}