CXX = g++
CXXFLAGS = -std=c++20 -g -Wall -Wextra -pedantic
TARGET = main
SRC = main.cpp JsonFile.cpp Exception.cpp InvalidAccessExp.cpp InvalidConversionExp.cpp InvalidTypeExp.cpp
HEADERS = JsonFile.hpp Exception.hpp InvalidAccessExp.hpp InvalidConversionExp.hpp InvalidTypeExp.hpp

# Default target
all: $(TARGET)

# Compile and link
$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean up
clean:
	rm -f $(TARGET)

# Enable debugging with gdb
debug: $(TARGET)
	gdb ./$(TARGET)
