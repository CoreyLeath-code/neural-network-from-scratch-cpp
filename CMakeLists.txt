cmake_minimum_required(VERSION 3.10)
project(NeuralNetworkCpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Professional Warning Flags
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

# Source Discovery: Finds all .cpp files in src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define Library
add_library(nn_core ${SOURCES})

# ROOT FIX: This allows your code to include "impl/..." because 'src' is now a root
target_include_directories(nn_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Executable
add_executable(nn_main src/main.cpp)
target_link_libraries(nn_main PRIVATE nn_core)

# Testing
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(test_neural_network tests/test_network.cpp)
target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

gtest_discover_tests(test_neural_network)
