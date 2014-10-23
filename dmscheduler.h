#ifndef __DMSCHEDULER_H__
#define __DMSCHEDULER_H__

#include "abstractscheduler.h"

class DMScheduler : public AbstractScheduler
{
    public:
        DMScheduler(const std::string &filename, unsigned int switch_percent_time);

    protected:
        virtual bool hasTaskPriority(const Task& a, const Task& b);
};

#endif