#include "consolelogger.h"
#include "svglogger.h"
#include "rmscheduler.h"
#include "dmscheduler.h"
#include "edfscheduler.h"
#include "fixedscheduler.h"
#include "llfscheduler.h"

#include <string>
#include <iostream>

#include <stdlib.h>

static void usage()
{
    std::cerr << "Usage : simEDF <task file> [<percent>] [--scheduler <scheduler>] [--steps <simulation time>] [--svg <filename>]" << std::endl;
}

static void printTime(const char *title, unsigned int time, unsigned int total)
{
    std::cout << title << time << " (" << (time * 100 / total) << " %)" << std::endl;
}

int main(int argc, char **argv)
{
    // Parameters
    std::string scheduler = "EDF";
    std::string filename;
    std::string svg_filename;
    int switch_percent_time = 0;
    int simulation_steps = -1;

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
    } else if (scheduler == "fixed") {
        sched = new FixedScheduler(filename, switch_percent_time);
    } else if (scheduler == "LLF") {
        sched = new LLFScheduler(filename, switch_percent_time);
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

    // Simulate the system
    if (simulation_steps == -1) {
        simulation_steps = sched->idealSimulationTime();
    }

    std::cout << "Predicted simulation time: " << simulation_steps << " time steps" << std::endl;
    sched->schedule(simulation_steps, logger);

    // Print statistics
    std::cout << std::endl;
    std::cout << "System schedulable: " << (sched->isSystemSchedulable() ? "yes" : "no") << std::endl;
    std::cout << "Simulation time: " << sched->currentTime() << " time steps" << std::endl;
    printTime("Active time: ", sched->activeTime(), sched->currentTime());
    printTime("Switching time: ", sched->switchingTime(), sched->currentTime());
    printTime("Idle time: ", sched->currentTime() - sched->activeTime() - sched->switchingTime(), sched->currentTime());
    std::cout << "Context switches: " << sched->numberPreemptions() << std::endl;

    delete logger;
    delete sched;

    return 0;
}
