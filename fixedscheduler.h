#ifndef __FIXEDCHEDULER_H__
#define __FIXEDCHEDULER_H__

#include "abstractscheduler.h"

class FixedScheduler : public AbstractScheduler
{
    public:
        FixedScheduler(const std::string &filename, unsigned int switch_percent_time);

    protected:
        virtual bool hasTaskPriority(const Task& a, const Task& b);
};

#endif