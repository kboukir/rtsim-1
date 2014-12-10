#include "dummylogger.h"

void DummyLogger::notifyTask(AbstractScheduler* scheduler, int task)
{
}

void DummyLogger::notifySwitch(AbstractScheduler* scheduler, unsigned int duration, unsigned int task)
{
}

void DummyLogger::notifyDeadlineMiss(AbstractScheduler* scheduler, int task)
{
}
