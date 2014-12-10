#ifndef __TASKFILEGENERATOR_H__
#define __TASKFILEGENERATOR_H__

#include <string>
#include "abstractscheduler.h"

class TaskFileGenerator {
    public:
        TaskFileGenerator(int number_tasks, int target_use_percent);
        void generateTaskFile(const std::string &filename);
        int getNumberTasks();
        int getTargetUsePercent();
        AbstractScheduler::Task *getTasks();
    private:
        int _number_tasks;
        int _target_use_percent;
        AbstractScheduler::Task *_tasks;
        static int random_number(int low, int high);
        
};


#endif