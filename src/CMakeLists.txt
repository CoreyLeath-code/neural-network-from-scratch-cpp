cmake_minimum_required(VERSION 3.16)

project(NeuralNetworkCpp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)

file(GLOB_RECURSE CORE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/impl/*.cpp"
)

if(NOT CORE_SOURCES)
    message(FATAL_ERROR "No implementation (.cpp) files found under src/impl")
endif()

add_library(nn_core STATIC ${CORE_SOURCES})

target_include_directories(nn_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(nn_main
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(nn_main
    PRIVATE
        nn_core
)

add_executable(test_neural_network
    ${CMAKE_CURRENT_SOURCE_DIR}/../tests/test_network.cpp
)

target_link_libraries(test_neural_network
    PRIVATE
        nn_core
        GTest::gtest_main
)

enable_testing()

include(GoogleTest)

gtest_discover_tests(test_neural_network)
