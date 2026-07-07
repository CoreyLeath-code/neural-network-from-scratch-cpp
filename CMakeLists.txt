cmake_minimum_required(VERSION 3.10)
project(NeuralNetworkCpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enforce professional warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

# 1. ADD THE PROJECT ROOT AS AN INCLUDE DIRECTORY
# This allows #include "core/..." to work because it sees the 'include' folder from root
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 2. Source Discovery
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")

# 3. Library
add_library(nn_core ${SOURCES} ${HEADERS})

# 4. Main Executable
add_executable(nn_main src/main.cpp)
target_link_libraries(nn_main PRIVATE nn_core)

# 5. Testing
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(test_neural_network tests/test_network.cpp)
target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

gtest_discover_tests(test_neural_network)
