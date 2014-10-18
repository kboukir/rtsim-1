CC=gcc
CXX=g++
CFLAGS=-g -Wall
CXXFLAGS=$(CFLAGS)

# Objects for all the executables
global_OBJECTS = abstractscheduler.o rmscheduler.o consolelogger.o

all: simEDF

clean:
	rm -f *.o simEDF

# simEDF
simEDF_OBJECTS = simEDF.o $(global_OBJECTS)

simEDF: $(simEDF_OBJECTS)
	$(CXX) -o simEDF $(simEDF_OBJECTS)

%.o: %.cpp
	$(CXX) -c -o $*.o $(CXXFLAGS) $*.cpp
