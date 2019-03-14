

# Mario is so lazy that has created this file to do his job


BIN_NAME := main	# name of the bin
SRC_EXT = cpp		# extension for the translation units
SRC_PATH = .		# path for the sources
SOURCES = terminator.cpp, selector.cpp, plotter.cpp
CXX ?= g++			# mpic++
CFLAGS = -std=c++11	# just in case
EXTRA_FLAGS = `root-config --cflags --glibs`

all: 
	$(CXX)  -o $(BIN_NAME) $(SOURCES) $(EXTRA_FLAGS)