#ifndef __RMSCHEDULER_H__
#define __RMSCHEDULER_H__

#include "abstractscheduler.h"

class RMScheduler : public AbstractScheduler
{
    public:
        RMScheduler(const std::string &filename, unsigned int switch_percent_time);

    protected:
        virtual int schedule();

    private:
        struct TaskInfo {
            unsigned int next_release_absolute;
            unsigned int executed_cycles;
        };

        std::vector<TaskInfo> _task_infos;
};

#endif