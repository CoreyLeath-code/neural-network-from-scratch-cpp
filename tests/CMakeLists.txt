cmake_minimum_required(VERSION 3.10)
project(NeuralNetworkCpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Since we are in src/, sources are just in impl/
file(GLOB_RECURSE CORE_SOURCES "impl/*.cpp")

# Define library
add_library(nn_core ${CORE_SOURCES})
# Include the current directory (src) so headers are found
target_include_directories(nn_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Executables: Look for main.cpp in the current directory (src)
add_executable(nn_main "main.cpp")
target_link_libraries(nn_main PRIVATE nn_core)

# Testing: Look for test_network.cpp by going up one level to root
add_executable(test_neural_network "../tests/test_network.cpp")
target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)
gtest_discover_tests(test_neural_network)
