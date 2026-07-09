cmake_minimum_required(VERSION 3.16)

project(NeuralNetworkCpp VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CTest)

option(NN_ENABLE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
option(NN_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(NN_ENABLE_COVERAGE "Enable GCC/Clang coverage instrumentation" OFF)
option(NN_BUILD_BENCHMARKS "Build benchmark executables" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/impl/*.cpp"
)

if(NOT CORE_SOURCES)
    message(FATAL_ERROR "No implementation (.cpp) files found under src/impl")
endif()

add_library(nn_core STATIC ${CORE_SOURCES})
add_library(NeuralNetworkCpp::nn_core ALIAS nn_core)

target_include_directories(nn_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/neural-network-cpp>
)

target_compile_features(nn_core PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(nn_core PRIVATE /W4 $<$<BOOL:${NN_ENABLE_WARNINGS_AS_ERRORS}>:/WX>)
else()
    target_compile_options(nn_core PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        $<$<BOOL:${NN_ENABLE_WARNINGS_AS_ERRORS}>:-Werror>
    )
endif()

if(NN_ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(nn_core PUBLIC -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(nn_core PUBLIC -fsanitize=address,undefined)
    else()
        message(FATAL_ERROR "NN_ENABLE_SANITIZERS requires GCC or Clang")
    endif()
endif()

if(NN_ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(nn_core PUBLIC --coverage -O0 -g)
        target_link_options(nn_core PUBLIC --coverage)
    else()
        message(FATAL_ERROR "NN_ENABLE_COVERAGE requires GCC or Clang")
    endif()
endif()

add_executable(nn_main main.cpp)
target_link_libraries(nn_main PRIVATE nn_core)

if(BUILD_TESTING)
    find_package(GTest REQUIRED)

    add_executable(test_neural_network
        ${CMAKE_CURRENT_SOURCE_DIR}/../tests/test_network.cpp
    )
    target_link_libraries(test_neural_network PRIVATE nn_core GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(test_neural_network)
endif()

if(NN_BUILD_BENCHMARKS)
    add_executable(benchmark_network
        ${CMAKE_CURRENT_SOURCE_DIR}/../benchmarks/benchmark_network.cpp
    )
    target_link_libraries(benchmark_network PRIVATE nn_core)
endif()

install(TARGETS nn_core nn_main
    EXPORT NeuralNetworkCppTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY core/ impl/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/neural-network-cpp
    FILES_MATCHING PATTERN "*.hpp"
)

set(CPACK_PACKAGE_NAME "neural-network-from-scratch-cpp")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "TGZ")
include(CPack)
