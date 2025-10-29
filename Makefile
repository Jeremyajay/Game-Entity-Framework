# Makefile for Program #3
# Jeremy Cuthbert CS302 Section 02

# Compiler
CXX = g++
CXXFLAGS = -Wall -g -std=c++17

# Executable name
TARGET = prog3

# Source files
SRCS = main.cpp npc.cpp derivednpc.cpp two_three_tree.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile .cpp files to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean

