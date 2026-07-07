cmake_minimum_required(VERSION 3.10)
project(NeuralNetworkCpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enforce professional warning flags
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

# Discover source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the core library
add_library(nn_core ${SOURCES})

# PUBLIC: This ensures that any target linking to nn_core 
# automatically inherits the 'src' directory as an include path.
target_include_directories(nn_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Define the main executable
add_executable(nn_main src/main.cpp)
target_link_libraries(nn_main PRIVATE nn_core)

# Define the testing suite
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(test_neural_network tests/test_network.cpp)
target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

gtest_discover_tests(test_neural_network)
