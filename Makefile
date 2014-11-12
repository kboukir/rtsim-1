CC=gcc
CXX=g++
CFLAGS=-g -Wall -std=c++0x
CXXFLAGS=$(CFLAGS)

# Objects for all the executables
global_OBJECTS = abstractscheduler.o rmscheduler.o dmscheduler.o edfscheduler.o consolelogger.o svglogger.o taskfilegenerator.o

all: simEDF taskGenerator

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

%.o: %.cpp
	$(CXX) -c -o $*.o $(CXXFLAGS) $*.cpp
