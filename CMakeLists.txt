cmake_minimum_required(VERSION 3.10)
project(neural_network_from_scratch_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
if(SOURCES)
    add_executable(neural_network ${SOURCES})
else()
    message(STATUS "No source files found in src/ directory")
endif()

# Enable testing if tests exist
enable_testing()
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
if(TEST_SOURCES)
    add_executable(run_tests ${TEST_SOURCES})
endif()
