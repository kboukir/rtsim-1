#ifndef __ABSTRACT_SCHEDULER_H__
#define __ABSTRACT_SCHEDULER_H__

#include <string>
#include <vector>

class AbstractLogger;

class AbstractScheduler
{
    public:
        /**
         * @brief Information about a task of the system
         */
        struct Task
        {
            unsigned int index;             /*!< @brief Index of the task in the system file */
            unsigned int offset;            /*!< @brief Time at which the first job of this task is released */
            unsigned int execution_time;    /*!< @brief Worst case execution time */
            unsigned int deadline;          /*!< @brief Deadline (relative, un number of ticks after the release time) */
            unsigned int period;            /*!< @brief Period at which a new job is to be released */

            int consumed_cycles;            /*!< @brief Number of cycles the last job of this task has already been run */
            int time_to_deadline;           /*!< @brief Cycles remaining before the next deadline */
            int time_to_release;            /*!< @brief Cycles remaining before the next release */
        };

    public:
        AbstractScheduler(const std::string &filename, unsigned int switch_percent_time);
        virtual ~AbstractScheduler();

        void schedule(unsigned int time_duration, AbstractLogger *logger);  /*!< @brief Runs N iterations of the scheduling algorithm */

        unsigned int taskCount() const;                 /*!< @brief Number of tasks in the system */
        int currentTask() const;                        /*!< @brief Index of the last task that was scheduled */
        const Task &task(int index) const;              /*!< @brief Return information about the i-th task */
        bool isTaskSchedulable(const Task &t) const;    /*!< @brief Returns true if a task has a released job that is not yet finished */

        unsigned int idealSimulationTime() const;       /*!< @brief Period during which the system should be simuled in order to decide whether it is schedulable or not */
        unsigned int currentTime() const;               /*!< @brief Current time tick (increments from 0 up to infinity as time passes) */
        bool isSystemSchedulable() const;
        unsigned int activeTime() const;
        unsigned int switchingTime() const;
        unsigned int numberPreemptions() const;

    protected:
        virtual int schedule();                         /*!< @brief Return which task must be executed for the current time step, or -1 if not task is schedulable */
        virtual bool hasTaskPriority(const Task &a,
                                     const Task &b) = 0;/*!< @brief Return true if task a should have a greater priority than task b */

        std::vector<Task> _tasks;

    private:
        bool nextTick(AbstractLogger *logger);          /*!< @brief Advance _current_time by one tick and update the tasks' remaining times. Return false if a deadline is missed */

    private:
        unsigned int _current_time;
        unsigned int _switch_percent_time;
        int _last_task_scheduled;

        // Statistics
        bool _system_schedulable;
        unsigned int _active_time;
        unsigned int _switching_time;
        unsigned int _number_preemptions;
};

#endif