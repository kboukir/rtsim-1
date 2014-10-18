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
            unsigned int offset;
            unsigned int execution_time;
            unsigned int deadline;
            unsigned int period;
        };

    public:
        AbstractScheduler(const std::string &filename, unsigned int switch_percent_time);
        virtual ~AbstractScheduler();

        bool schedule(unsigned int time_duration, AbstractLogger *logger);  /*!< @brief Runs N iterations of the scheduling algorithm */

        unsigned int taskCount() const;                 /*!< @brief Number of tasks in the system */
        int currentTask() const;               /*!< @brief Index of the last task that was scheduled */
        const Task &task(int index) const;     /*!< @brief Return information about the i-th task */

        unsigned int currentTime() const;               /*!< @brief Current time tick (increments from 0 up to infinity as time passes) */

    protected:
        virtual int schedule() = 0;            /*!< @brief Return which task must be executed for the current time step, or -1 if not task is schedulable */

    private:
        std::vector<Task> _tasks;
        unsigned int _current_time;
        unsigned int _switch_percent_time;
        int _last_task_scheduled;
};

#endif