# Basic Makefile for bitfake project

CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++17 -O2
LDFLAGS = -lavformat -lavcodec -lavutil

SRC = Main.cpp MainFunctions.cpp
OBJ = $(SRC:%.cpp=sources/%.o)
TARGET = sources/bitfake

all: sources_dir $(TARGET)

sources_dir:
	mkdir -p sources


$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sources/%.o: %.cpp | sources_dir
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f sources/*.o sources/bitfake

.PHONY: all clean sources_dir
