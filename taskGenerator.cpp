#include "taskfilegenerator.h"

#include <string>
#include <iostream>

#include <stdlib.h>

static void usage()
{
    std::cerr << "Usage : taskGenerator -u <use> -n <number tasks> <task file>" << std::endl;
}

int main(int argc, char **argv)
{
    // Parameters
    int target_use_percent = 50;
    int number_tasks = 3;
    std::string filename;

    enum {
        TaskFile,
        UsagePercent,
        NumberTasks
    } state = TaskFile; // The first parameter is expected to be the task file

    for (int i=1; i<argc; ++i) {
        std::string arg(argv[i]);

        if (arg == "-u") {
            state = UsagePercent;
            continue;
        } else if (arg == "-n") {
            state = NumberTasks;
            continue;
        }

        switch (state) {
            case TaskFile:
                filename = arg;
                break;

            case UsagePercent:
                target_use_percent = atoi(arg.c_str());
                state = TaskFile;
                break;

            case NumberTasks:
                number_tasks = atoi(arg.c_str());
                state = TaskFile;
                break;
        }
    }

    // Check that the task file is present
    if (filename.size() == 0) {
        usage();
        return 1;
    }

    // Generate the task file
    TaskFileGenerator taskfilegenerator(number_tasks, target_use_percent);
    taskfilegenerator.generateTaskFile(filename);

    return 0;
}
