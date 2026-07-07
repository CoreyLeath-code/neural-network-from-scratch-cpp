cmake_minimum_required(VERSION 3.10)
project(NeuralNetworkCpp VERSION 1.0)

# 1. Standard Enforcement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Professional Warning Flags (L6 Standard)
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

# 3. Dynamic Source Discovery
# This finds all .cpp files in src/ and include/ automatically
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")

# 4. Library Definition
add_library(nn_core ${SOURCES} ${HEADERS})
target_include_directories(nn_core PUBLIC include)

# 5. Executable Definition
add_executable(nn_main src/main.cpp)
target_link_libraries(nn_main PRIVATE nn_core)

# 6. Testing Framework (GoogleTest)
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

# Define Test Executable
add_executable(test_neural_network tests/test_network.cpp)
target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

# Register the test
gtest_discover_tests(test_neural_network)
