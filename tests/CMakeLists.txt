cmake_minimum_required(VERSION 3.10)
project(NeuralNetworkCpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Explicitly list source files from the root perspective
# We use file(GLOB ...) to find them based on the current location
file(GLOB_RECURSE CORE_SOURCES "src/impl/*.cpp")

# 2. Define Library
add_library(nn_core ${CORE_SOURCES})
target_include_directories(nn_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# 3. Executable Definitions (Paths are now relative to the root)
add_executable(nn_main "src/main.cpp")
target_link_libraries(nn_main PRIVATE nn_core)

# 4. Testing
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(test_neural_network "tests/test_network.cpp")
target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

gtest_discover_tests(test_neural_network)
