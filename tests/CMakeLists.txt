cmake_minimum_required(VERSION 3.10)
project(NeuralNetworkCpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Discover all source files in src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the library and make the project root available for includes
add_library(nn_core ${SOURCES})
target_include_directories(nn_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Define the main executable
add_executable(nn_main "src/main.cpp")
target_link_libraries(nn_main PRIVATE nn_core)

# Testing setup
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(test_neural_network "tests/test_network.cpp")
target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

gtest_discover_tests(test_neural_network)
