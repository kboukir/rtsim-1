#ifndef __LLFSCHEDULER_H__
#define __LLFSCHEDULER_H__

#include "abstractscheduler.h"

class LLFScheduler : public AbstractScheduler
{
    public:
        LLFScheduler(const std::string &filename, unsigned int switch_percent_time);

    protected:
        virtual bool hasTaskPriority(const Task& a, const Task& b);

    private:
        int laxity(const Task &t);
};

#endif