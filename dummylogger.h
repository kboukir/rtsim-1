#infdef __DUMMYLOGGER_H__
#define __DUMMYLOGGER_H__

class DummyLogger : public AbstractLogger {
	public:
        DummyLogger() {}
        virtual ~DummyLogger() {}

        /**
         * @brief Notify that task @p task is being run by the @p scheduler
         *
         * @p scheduler can be used to retrieve the current time or the list of
         * other tasks.
         *
         * @note task can be -1 if there is no task scheduled during this time slot
         */
        virtual void notifyTask(AbstractScheduler *scheduler, int task);

        /**
         * @brief Notify that there has been a switching time (time lost)
         */
        virtual void notifySwitch(AbstractScheduler *scheduler, unsigned int duration, unsigned int task);

        /**
         * @brief Notify that a deadline for task @p task has been missed
         */
        virtual void notifyDeadlineMiss(AbstractScheduler *scheduler, int task);
};

#endif
