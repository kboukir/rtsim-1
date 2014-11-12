#include "consolelogger.h"
#include "svglogger.h"
#include "rmscheduler.h"
#include "dmscheduler.h"
#include "edfscheduler.h"

#include <string>
#include <iostream>

#include <stdlib.h>

static void usage()
{
    std::cerr << "Usage : simEDF <task file> [<percent>] [--scheduler <scheduler>] [--steps <simulation time>] [--svg <filename>]" << std::endl;
}

int main(int argc, char **argv)
{
    // Parameters
    std::string scheduler = "EDF";
    std::string filename;
    std::string svg_filename;
    int switch_percent_time = 0;
    int simulation_steps = 400;

    enum {
        None,
        TaskFile,
        UsagePercent,
        SimulationSteps,
        Scheduler,
        SvgFileName
    } state = TaskFile; // The first parameter is expected to be the task file

    for (int i=1; i<argc; ++i) {
        std::string arg(argv[i]);

        if (arg == "--scheduler") {
            state = Scheduler;
            continue;
        } else if (arg == "--steps") {
            state = SimulationSteps;
            continue;
        } else if (arg == "--svg") {
            state = SvgFileName;
            continue;
        }

        switch (state) {
            case TaskFile:
                filename = arg;
                state = UsagePercent;
                break;

            case UsagePercent:
                switch_percent_time = atoi(arg.c_str());
                state = None;
                break;

            case Scheduler:
                scheduler = arg;
                state = None;
                break;

            case SimulationSteps:
                simulation_steps = atoi(arg.c_str());
                state = None;
                break;

            case SvgFileName:
                svg_filename = arg;
                state = None;
                break;

            case None:
                usage();
                break;
        }
    }

    // Check that the task file is present
    if (filename.size() == 0) {
        usage();
        return 1;
    }

    // Instantiate the right scheduler
    AbstractScheduler *sched = 0;

    if (scheduler == "RM") {
        sched = new RMScheduler(filename, switch_percent_time);
    } else if (scheduler == "DM") {
        sched = new DMScheduler(filename, switch_percent_time);
    } else if (scheduler == "EDF") {
        sched = new EDFScheduler(filename, switch_percent_time);
    } else {
        std::cerr << "Unknown scheduler name: " << scheduler << std::endl;
        return 1;
    }

    // Instantiate the right logger
    AbstractLogger *logger = 0;

    if (svg_filename.empty()) {
        logger = new ConsoleLogger;
    } else {
        logger = new SvgLogger(svg_filename);
    }

    if (sched->schedule(simulation_steps, logger)) {
        std::cout << "System schedulable" << std::endl;
    } else {
        std::cout << "System NOT schedulable" << std::endl;
    }

    delete logger;
    delete sched;

    return 0;
}
