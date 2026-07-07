cmake_minimum_required(VERSION 3.10)
project(NeuralNetworkCpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(nn_core ${SOURCES})

# This is the most critical line. It tells the compiler:
# "When searching for headers, look inside the 'include' folder."
target_include_directories(nn_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(test_neural_network tests/test_network.cpp)
target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

# GoogleTest setup
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)
gtest_discover_tests(test_neural_network)