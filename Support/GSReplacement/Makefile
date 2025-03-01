# Makefile for GameSpy Replacement
# Note: This project requires Windows APIs and should be built on Windows
# This Makefile is provided mainly for documentation purposes

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I./include
LDFLAGS = -lws2_32

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Output files
DLL_TARGET = $(BIN_DIR)/gamespy.dll
EXE_TARGET = $(BIN_DIR)/GSReplacementApp.exe

# Default target
all: notice

# Notice about Windows requirement
notice:
	@echo "This project requires Windows APIs and should be built on Windows."
	@echo "Please use the build.bat script on a Windows system or use CMake."
	@echo "For cross-compilation, consider using MinGW or a Windows VM."
	@echo ""
	@echo "On Windows, you can build with:"
	@echo "  mkdir build"
	@echo "  cd build"
	@echo "  cmake .."
	@echo "  cmake --build . --config Release"
	@echo ""
	@echo "To force build anyway, use: make force-build"

# Force build target (will likely fail on non-Windows systems)
force-build: directories $(DLL_TARGET) $(EXE_TARGET)

# Create directories
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link DLL
$(DLL_TARGET): $(OBJECTS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

# Link executable
$(EXE_TARGET): $(OBJECTS)
	$(CXX) -DSTANDALONE_APP -o $@ $^ $(LDFLAGS)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all notice force-build directories clean 