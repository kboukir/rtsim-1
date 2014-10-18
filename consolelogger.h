#ifndef __CONSOLELOGGER_H__
#define __CONSOLELOGGER_H__

#include "abstractlogger.h"

class ConsoleLogger : public AbstractLogger
{
    public:
        virtual void notifyTask(AbstractScheduler *scheduler, int task);
        virtual void notifySwitch(AbstractScheduler *scheduler, unsigned int duration, unsigned int task);

    private:
        void printTime(unsigned int time);
        void printTaskPrefix(AbstractScheduler *scheduler, unsigned int task, unsigned int time);
};

#endif