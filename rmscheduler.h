#ifndef __RMSCHEDULER_H__
#define __RMSCHEDULER_H__

#include "abstractscheduler.h"

class RMScheduler : public AbstractScheduler
{
    public:
        RMScheduler(const std::string &filename, unsigned int switch_percent_time);

    protected:
        virtual bool hasTaskPriority(const Task& a, const Task& b);
};

#endif