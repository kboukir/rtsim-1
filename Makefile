CC=gcc
CXX=g++
CFLAGS=-g -Wall -std=c++0x
CXXFLAGS=$(CFLAGS)

# Objects for all the executables
global_OBJECTS = abstractscheduler.o rmscheduler.o dmscheduler.o edfscheduler.o llfscheduler.o fixedscheduler.o consolelogger.o svglogger.o taskfilegenerator.o graph2d.o dummylogger.o

all: simEDF taskGenerator EDF_study

clean:
	rm -f *.o simEDF taskGenerator

# simEDF
simEDF_OBJECTS = simEDF.o $(global_OBJECTS)

simEDF: $(simEDF_OBJECTS)
	$(CXX) -o simEDF $(simEDF_OBJECTS)

# taskGenerator
taskGenerator_OBJECTS = taskGenerator.o $(global_OBJECTS)

taskGenerator: $(taskGenerator_OBJECTS)
	$(CXX) -o taskGenerator $(taskGenerator_OBJECTS)

#EDF_study
EDF_study_OBJECTS = EDF_study.o $(global_OBJECTS)

EDF_study : $(EDF_study_OBJECTS)
	$(CXX) -o EDF_study $(EDF_study_OBJECTS)

%.o: %.cpp
	$(CXX) -c -o $*.o $(CXXFLAGS) $*.cpp
