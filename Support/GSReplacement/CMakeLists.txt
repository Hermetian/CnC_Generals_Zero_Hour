cmake_minimum_required(VERSION 3.10)
project(GSReplacement VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if we're on Windows
if(NOT WIN32)
    message(FATAL_ERROR "This project is Windows-specific and requires Windows APIs")
endif()

# Include directories
include_directories(include)

# Source files for the DLL
set(SOURCE_FILES
    src/main.cpp
    src/Logger.cpp
    src/ConfigParser.cpp
    src/GameBackend.cpp
    src/RevoraBackend.cpp
    src/FunctionHook.cpp
    src/GameSpyProxy.cpp
)

# Create the DLL
add_library(gamespy SHARED ${SOURCE_FILES})

# Link with necessary libraries
target_link_libraries(gamespy ws2_32)

# Create a standalone executable
add_executable(GSReplacementApp ${SOURCE_FILES})
target_link_libraries(GSReplacementApp ws2_32)

# Define preprocessor flags for the executable
target_compile_definitions(GSReplacementApp PRIVATE STANDALONE_APP)

# Install the files
install(TARGETS gamespy DESTINATION bin)
install(TARGETS GSReplacementApp DESTINATION bin)
install(FILES README.md DESTINATION .) 