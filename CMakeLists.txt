cmake_minimum_required(VERSION 3.16)
project(neural_network_from_scratch_cpp VERSION 1.0.0 LANGUAGES CXX)

include(CTest)
include(GNUInstallDirs)

option(NN_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(NN_ENABLE_COVERAGE "Enable GCC/Clang coverage instrumentation" OFF)
option(NN_ENABLE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)

add_library(neuralnet
    src/impl/Activations.cpp
    src/impl/CSVLoader.cpp
    src/impl/DenseLayer.cpp
    src/impl/Losses.cpp
    src/impl/NeuralNet.cpp)
target_include_directories(neuralnet PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_compile_features(neuralnet PUBLIC cxx_std_17)

if(MSVC)
  target_compile_options(neuralnet PRIVATE /W4 $<$<BOOL:${NN_ENABLE_WARNINGS_AS_ERRORS}>:/WX>)
else()
  target_compile_options(neuralnet PRIVATE -Wall -Wextra -Wpedantic
      $<$<BOOL:${NN_ENABLE_WARNINGS_AS_ERRORS}>:-Werror>)
endif()

if(NN_ENABLE_SANITIZERS AND NOT MSVC)
  target_compile_options(neuralnet PUBLIC -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(neuralnet PUBLIC -fsanitize=address,undefined)
endif()

if(NN_ENABLE_COVERAGE AND NOT MSVC)
  target_compile_options(neuralnet PUBLIC --coverage -O0 -g)
  target_link_options(neuralnet PUBLIC --coverage)
endif()

add_executable(neural_network src/main.cpp)
target_link_libraries(neural_network PRIVATE neuralnet)

add_executable(benchmark_network benchmarks/benchmark_network.cpp)
target_link_libraries(benchmark_network PRIVATE neuralnet)

if(BUILD_TESTING)
  find_package(GTest REQUIRED)
  add_executable(test_neural_network tests/test_network.cpp)
  target_link_libraries(test_neural_network PRIVATE neuralnet GTest::gtest_main)
  include(GoogleTest)
  gtest_discover_tests(test_neural_network)
endif()

install(TARGETS neuralnet neural_network benchmark_network
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY src/core src/impl DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/neuralnet
    FILES_MATCHING PATTERN "*.hpp")

set(CPACK_PACKAGE_NAME neural-network-from-scratch-cpp)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR TGZ)
include(CPack)
