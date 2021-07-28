.PHONY: all run clean

CXX = g++
FLAGS = -Wall -O0 -c -Iinclude
LINKING= 

BUILD_DIRECTORY = build
SOURCE_DIRECTORY = src
EXECUTABLE_DIRECTORY = bin
EXECUTABLE = tibasic

SOURCES:=$(wildcard $(SOURCE_DIRECTORY)/*.cpp)
OBJ:=$(patsubst $(SOURCE_DIRECTORY)/%.cpp,$(BUILD_DIRECTORY)/%.o,$(SOURCES))

# Creates the directory if it doesn't exist and compile needed files.
all: $(BUILD_DIRECTORY) $(EXECUTABLE_DIRECTORY) $(EXECUTABLE_DIRECTORY)/$(EXECUTABLE)


# Creates the build directory if it doesn't exist
$(BUILD_DIRECTORY):
	mkdir -p $(BUILD_DIRECTORY)

# Creates the executable directory if it doesn't exist
$(EXECUTABLE_DIRECTORY):
	mkdir -p $(EXECUTABLE_DIRECTORY)


# Links object files into executable
$(EXECUTABLE_DIRECTORY)/$(EXECUTABLE): $(OBJ)
	$(CXX) $^ -o $@ $(LINKING)


# Compiles .cpp files into respective .o files.
$(OBJ): $(BUILD_DIRECTORY)/%.o : $(SOURCE_DIRECTORY)/%.cpp
	$(CXX) $(FLAGS) $< -o $@


# Runs the executable
run:
	./$(EXECUTABLE_DIRECTORY)/$(EXECUTABLE)

# Cleans the executable and object files.
clean:
	rm -rf $(BUILD_DIRECTORY)/*.o $(EXECUTABLE_DIRECTORY)/$(EXECUTABLE)
