cmake_minimum_required(VERSION 3.10)
project(NeuralNetworkCpp VERSION 1.0)

# Enforce C++17 for modern features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Professional Warning Flags (L6 Standard)
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

# Define Library
add_library(nn_core src/network.cpp)
target_include_directories(nn_core PUBLIC include)

# Define Executable
add_executable(nn_main src/main.cpp)
target_link_libraries(nn_main PRIVATE nn_core)
# Enable testing
enable_testing()

# Find GoogleTest
find_package(GTest REQUIRED)
include(GoogleTest)

# Define Test Executable
add_executable(test_neural_network tests/test_network.cpp)
target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

# Register the test
gtest_discover_tests(test_neural_network)
