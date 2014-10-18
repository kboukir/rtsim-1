#ifndef __ABSTRACTLOGGER_H__
#define __ABSTRACTLOGGER_H__

class AbstractScheduler;

/**
 * @brief Receives notifications about what happens in a scheduler
 *
 * Subclasses of this interface can print a log to the console, generate a SVG
 * file or anything else.
 */
class AbstractLogger
{
    public:
        AbstractLogger() {}
        virtual ~AbstractLogger() {}

        /**
         * @brief Notify that task @p task is being run by the @p scheduler
         *
         * @p scheduler can be used to retrieve the current time or the list of
         * other tasks.
         *
         * @note task can be -1 if there is no task scheduled during this time slot
         */
        virtual void notifyTask(AbstractScheduler *scheduler, int task) = 0;

        /**
         * @brief Notify that there has been a switching time (time lost)
         */
        virtual void notifySwitch(AbstractScheduler *scheduler, unsigned int duration, unsigned int task) = 0;
};

#endif