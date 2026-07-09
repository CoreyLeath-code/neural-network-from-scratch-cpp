cmake_minimum_required(VERSION 3.10)
project(neural_network_from_scratch_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories - add both include and src
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

# Collect source files (exclude tests and benchmarks)
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*test.*")

# Create main executable
if(SOURCES)
    add_executable(neural_network ${SOURCES})
    target_include_directories(neural_network PRIVATE ${PROJECT_SOURCE_DIR}/src)
else()
    message(STATUS "No source files found in src/ directory")
endif()

# Enable testing
enable_testing()

# Find GoogleTest
find_package(GTest REQUIRED)

# Create test executable
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
if(TEST_SOURCES)
    add_executable(run_tests ${TEST_SOURCES} ${SOURCES})
    target_include_directories(run_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)
    target_link_libraries(run_tests PRIVATE GTest::gtest_main)
    add_test(NAME run_tests COMMAND run_tests)
endif()
