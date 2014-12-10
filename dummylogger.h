#ifndef __DUMMYLOGGER_H__
#define __DUMMYLOGGER_H__

#include "abstractlogger.h"

class DummyLogger : public AbstractLogger {
    public:
        virtual ~DummyLogger() {}

        
        virtual void notifyTask(AbstractScheduler *scheduler, int task);
        virtual void notifySwitch(AbstractScheduler *scheduler, unsigned int duration, unsigned int task);
        virtual void notifyDeadlineMiss(AbstractScheduler *scheduler, int task);
};

#endif
