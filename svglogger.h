#ifndef __SVGLOGGER_H__
#define __SVGLOGGER_H__

#include "abstractlogger.h"

#include <string>
#include <list>
#include <fstream>

class SvgLogger : public AbstractLogger
{
    public:
        SvgLogger(const std::string &filename);
        virtual ~SvgLogger();

        virtual void notifyTask(AbstractScheduler *scheduler, int task);
        virtual void notifySwitch(AbstractScheduler *scheduler, unsigned int duration, unsigned int task);
        virtual void notifyDeadlineMiss(AbstractScheduler *scheduler, int task);

    private:
        enum Activity
        {
            Processing,
            Switching,
            DeadlineMiss
        };

        struct Rect {
            unsigned int start_time;    /*!< @brief Time at which the rectangle starts */
            unsigned int len;           /*!< @brief Duration of the rectangle in time ticks */
            Activity activity;          /*!< @brief Activity performed during this rectangle */

            unsigned int endTime() const
            {
                return start_time + len;
            }
        };

        void notifyActivity(AbstractScheduler *scheduler,
                            int task,
                            unsigned int time,
                            Activity activity);     /*!< @brief Extend or create a rectangle for @p activity in @p task */
        unsigned int timeToX(unsigned int time);    /*!< @brief Take a time tick and return an SVG x coordinate */

        /**
         * @brief Draw a rectangle in the SVG file using the provided @p style SVG style string
         */
        void drawRectangle(std::ofstream &svg,
                           unsigned int task,
                           const Rect &r,
                           const std::string &style);

    private:
        std::list<Rect> *_rectangles;   /*!< @brief List of rectangles for each task */
        AbstractScheduler *_scheduler;
        std::string _filename;
};

#endif