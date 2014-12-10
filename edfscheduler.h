#ifndef __EDFSCHEDULER_H__
#define __EDFSCHEDULER_H__

#include "abstractscheduler.h"


class EDFScheduler : public AbstractScheduler
{
    public:
        EDFScheduler(const std::string &filename, unsigned int switch_percent_time);
        EDFScheduler(std::vector<AbstractScheduler::Task> tasks, unsigned int switch_percent_time);

    protected:
        virtual bool hasTaskPriority(const Task& a, const Task& b);
};

#endif
