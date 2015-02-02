# Compiling

Typing `make` in a console should work, rtsim has no external dependency (beyond a working C++11 compiler)

# Generating example graphs

In the `slides` directory, type `./generate.sh`. This will generate a SVG file for each system in the directory. Modify `generate.sh` in order to tune the simulation and output parameters.

# System description file format

A system represents a list of periodic tasks and is described in a .system file. Those files are very simple: each line represents a task (numbered in order, the first line being T1, the second being T2, etc), and each task is described by four space-separated numbers:

* The offset (in the case of an asynchronous system)
* The period giving the times at which jobs are released
* The relative deadline
* The worst-case execution time

Typing `./simEDF` in a console, without any parameter, prints a short help message listing the arguments of the command. Available schedulers are RM (rate-monotonic), DM (deadline-monotonic), EDF (earliest deadline first), LLF (least laxity first) and fixed (T1 has a greater priority than T2, etc).

# Converting SVG files to PDF files

SVG files are easy to generate but are not practical to integrate in a LaTeX document or syllabus. Converting SVG files to PDF files is easy if Inkscape is installed. Just type:

    inkscape -A X.pdf X.svg
