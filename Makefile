

# Mario is so lazy that has created this file to do his job (well, has )

BIN_NAME := main	# name of the bin
SRC_EXT = cpp		# extension for the translation units
SRC_PATH = .		# path for the sources
SOURCES = terminator.cpp  selector.cpp  plotter.cpp
OBJECTS = terminator.o selector.o plotter.o
HEADERS = selector.h, plotter.h
CXX ?= g++			# serial
MPI ?= mpic++		# parallel
CFLAGS = -std=c++11	# just in case
EXTRA_FLAGS = `root-config --cflags --glibs`


#serialObj:
#	$(CXX)  -o $(BIN_NAME) $(OBJECTS) $(EXTRA_FLAGS)

serialBin: $(OBJECTS)
	$(CXX)  -c $(SOURCES)


# Why I cannot find the root dependencies anywhereeee!
terminator.o: $(SOURCES) $(HEADERS)
	g++ -c termiator.cpp

selector.o: $(SOURCES) $(HEADERS)
	g++ -c selector.cpp

plotter.o: $(SOURCES) $(HEADERS)
	g++ -c plotter.cpp

all: 
	$(CXX)  -c $(BIN_NAME) $(SOURCES) $(EXTRA_FLAGS)

#Nvm, use CMake instead