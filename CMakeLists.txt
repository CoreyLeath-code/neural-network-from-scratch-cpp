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

# Source Discovery
file(GLOB_RECURSE SOURCES "src/*.cpp")
# Note: Usually you do not need to add headers to add_library, but we include them for IDE support
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")

# Library Definition
add_library(nn_core ${SOURCES} ${HEADERS})
# This makes the 'include' folder available to anyone linking to nn_core
target_include_directories(nn_core PUBLIC include)

# Main Executable
add_executable(nn_main src/main.cpp)
target_link_libraries(nn_main PRIVATE nn_core)

# Testing Framework
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

# Test Executable
add_executable(test_neural_network tests/test_network.cpp)
# Ensure the test target also sees the include directory
target_include_directories(test_neural_network PRIVATE include)
target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

gtest_discover_tests(test_neural_network)
