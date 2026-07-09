cmake_minimum_required(VERSION 3.10)
project(NeuralNetworkCpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Explicitly add your files one by one. This is the most reliable way.
add_library(nn_core 
    src/impl/Activations.cpp 
    src/impl/CSVLoader.cpp 
    src/impl/DenseLayer.cpp 
    src/impl/Losses.cpp 
    src/impl/NeuralNet.cpp
)

target_include_directories(nn_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(nn_main src/main.cpp)
target_link_libraries(nn_main PRIVATE nn_core)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(test_neural_network tests/test_network.cpp)
target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

gtest_discover_tests(test_neural_network)
