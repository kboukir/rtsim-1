#include "svglogger.h"
#include "abstractscheduler.h"

#include <fstream>

SvgLogger::SvgLogger(const std::string &filename)
: _rectangles(nullptr),
  _scheduler(nullptr),
  _filename(filename),
  _max_time(0)
{
}

SvgLogger::~SvgLogger()
{
    std::ofstream svg(_filename.c_str());

    // SVG header
    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    svg << "<svg width=\"" << (timeToX(_max_time) + 30) << "\" "
        << "height=\"" << (_scheduler->taskCount() * 20 + 30) << "\" "
        << "version=\"1.1\" xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\">\n";

    // SVG defs. Here we define the gradients and effects applied to the rectangles
    svg << "\
 <defs>\n\
  <linearGradient id=\"grad1\" x1=\"0\" y1=\"0\" x2=\"0\" y2=\"1\">\n\
   <stop style=\"stop-color:#E8B7D7;stop-opacity:1;\" offset=\"0\" />\n\
   <stop style=\"stop-color:#F491DF;stop-opacity:1;\" offset=\"1\" />\n\
  </linearGradient>\n\
  <linearGradient id=\"grad2\" x1=\"0\" y1=\"0\" x2=\"0\" y2=\"1\">\n\
   <stop style=\"stop-color:#A4C0E4;stop-opacity:1;\" offset=\"0\" />\n\
   <stop style=\"stop-color:#6193CF;stop-opacity:1;\" offset=\"1\" />\n\
  </linearGradient>\n\
  <linearGradient id=\"grad3\" x1=\"0\" y1=\"0\" x2=\"0\" y2=\"1\">\n\
   <stop style=\"stop-color:#FFF6C8;stop-opacity:1;\" offset=\"0\" />\n\
   <stop style=\"stop-color:#FFF299;stop-opacity:1;\" offset=\"1\" />\n\
  </linearGradient>\n\
  <linearGradient id=\"grad4\" x1=\"0\" y1=\"0\" x2=\"0\" y2=\"1\">\n\
   <stop style=\"stop-color:#BFFFBF;stop-opacity:1;\" offset=\"0\" />\n\
   <stop style=\"stop-color:#80FF80;stop-opacity:1;\" offset=\"1\" />\n\
  </linearGradient>\n\
  <filter id=\"blur\">\n\
   <feGaussianBlur stdDeviation=\"2.85\" />\n\
  </filter>\n\
 </defs>\n\
";
    svg << " <g>\n";

    // Names of the tasks
    unsigned int height = _scheduler->taskCount() * 20 + 5;

    for (unsigned int i=0; i<_scheduler->taskCount(); ++i) {
        svg << "  <text "
            << "x=\"0\" "
            << "y=\"" << (i * 20 + 15) << "\" "
            << "style=\"font-size:13px;\">T" << (i + 1) << "</text>\n";
    }

    svg << "  <path d=\"M 25,0 25," << height << "\" style=\"stroke-width:1;stroke:#000000;\" />\n";
    svg << "  <path d=\"M 25," << height << " " << (timeToX(_max_time) + 30) << "," << height << "\" style=\"stroke-width:1;stroke:#000000;\" />\n";

    // Vertical lines
    for (unsigned int i=0; i<_max_time; i+= 10) {
        unsigned int x = timeToX(i) + 30;

        svg << "  <path d=\"M " << x << ",0 " << x << ", " << height << "\" style=\"stroke-width:1;stroke:#888888;stroke-dasharray:4,4\" />\n";

        if (i % 20 == 0) {
            svg << "  <text "
                << "x=\"" << x << "\" "
                << "y=\"" << (height + 25) << "\" "
                << "style=\"font-size:13px;text-anchor:middle;\">" << i << "</text>\n";
        }
    }

    // Draw the rectangles of the tasks
    static const char *gradientNames[4] = {
        "grad1",
        "grad2",
        "grad3",
        "grad4"
    };

    for (unsigned int i=0; i<_scheduler->taskCount(); ++i) {
        const std::list<Rect> &rectangles = _rectangles[i];

        for (const auto &rectangle : rectangles) {
            switch (rectangle.activity) {
            case Processing:
                // There is a shadow below the rectangle
                drawRectangle(
                    svg,
                    i,
                    rectangle,
                    "color:#000000;fill:#000000;opacity:0.3;filter:url(#blur);",
                    2,
                    2
                );

                // Actual rectangle
                drawRectangle(
                    svg,
                    i,
                    rectangle,
                    std::string("fill:url(#") + gradientNames[i % 4] + ");stroke:#000000;stroke-width:1;"
                );
                break;

            case Switching:
                drawRectangle(
                    svg,
                    i,
                    rectangle,
                    "fill:#aaaaaa;"
                );
                break;

            case DeadlineMiss:
                drawRectangle(
                    svg,
                    i,
                    rectangle,
                    "stroke-width:2;stroke:#ff0000;",
                    -1
                );
                break;
            }
        }
    }

    // Draw the release times and the deadlines of all the tasks
    for (unsigned int i=0; i<_scheduler->taskCount(); ++i) {
        const AbstractScheduler::Task &t = _scheduler->task(i);

        for (unsigned int time=t.offset; time<_max_time; time += t.period) {
            unsigned int x = timeToX(time) + 30;
            unsigned int x2 = x + timeToX(t.deadline);
            unsigned int y = i * 20;

            svg << "  <path d=\"M " << x << "," << y << " " << x << "," << (y + 20) << "\" style=\"stroke-width:1;stroke:#000000;marker-end:url(#Arrow1Lend);\" />\n";
            svg << "  <rect x=\"" << (x2 - 3) << "\" y=\"" << (y + 14) << "\" width=\"6\" height=\"6\" style=\"fill:#ffffff;stroke-width:1;stroke:#000000;\" />\n";
        }
    }

    // SVG footer
    svg << " </g>\n";
    svg << "</svg>\n";

    svg.flush();
    svg.close();

    // Don't leak the rectangles
    if (_rectangles) {
        delete[] _rectangles;
    }
}

unsigned int SvgLogger::timeToX(unsigned int time)
{
    return time * 2;
}

void SvgLogger::drawRectangle(std::ofstream &svg,
                              unsigned int task,
                              const Rect &r,
                              const std::string &style,
                              int dx,
                              int dy)
{
    svg << "  <rect "
        << "x=\"" << ((int)timeToX(r.start_time) + 30 + dx) << "\" "
        << "y=\"" << ((int)task * 20 + dy) << "\" "
        << "width=\"" << timeToX(r.len) << "\" "
        << "height=\"20\" "
        << "style=\"" + style + "\" />\n";
}

void SvgLogger::notifyTask(AbstractScheduler *scheduler, int task)
{
    notifyActivity(scheduler, task, scheduler->currentTime(), Processing);
}

void SvgLogger::notifySwitch(AbstractScheduler *scheduler, unsigned int duration, unsigned int task)
{
    for (unsigned int i=0; i<duration; ++i) {
        unsigned int time = scheduler->currentTime() + i;

        notifyActivity(scheduler, scheduler->currentTask(), time, Switching);
        notifyActivity(scheduler, task, time, Switching);
    }
}

void SvgLogger::notifyDeadlineMiss(AbstractScheduler *scheduler, int task)
{
    notifyActivity(scheduler, task, scheduler->currentTime(), DeadlineMiss);
}

void SvgLogger::notifyActivity(AbstractScheduler *scheduler,
                               int task,
                               unsigned int time,
                               Activity activity)
{
    _scheduler = scheduler;
    _max_time = std::max(_max_time, time);

    if (task == -1) {
        return;
    }

    // Create the array of rectangle lists
    if (!_rectangles) {
        _rectangles = new std::list<Rect>[_scheduler->taskCount()];
    }

    // List of rectangles of the given task
    std::list<Rect> &rectangles = _rectangles[task];

    if (rectangles.size() == 0 ||
        rectangles.back().activity != activity ||
        rectangles.back().endTime() != time) {
        // Need to add a new rectangle to the list
        Rect r;

        r.activity = activity;
        r.start_time = time;
        r.len = 1;

        rectangles.push_back(r);
    } else {
        // The last rectangle can be extended
        rectangles.back().len++;
    }
}
