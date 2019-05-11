cmake_minimum_required(VERSION 3.1)

project(SerfCpp)

set(VERSION "0.4.0")

if(NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

#---------------------------------------------------------------------------------------
# compiler config
#---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH}
                        ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

#---------------------------------------------------------------------------------------
# Dependencies
#---------------------------------------------------------------------------------------
find_package(Threads)
find_package(GTest)
find_package(MsgPackC 3.1.0 REQUIRED)

if (GTEST_FOUND)
  option (SERF_CPP_BUILD_TESTS "Build serf-cpp tests." ON)
endif (GTEST_FOUND)

option (SERF_CPP_BUILD_EXAMPLE "Build example test driver" ON)

set(SERF_CPP_BUILD_COVERAGE false CACHE BOOL "Build for code coverage")
 
add_subdirectory(src)

if (SERF_CPP_BUILD_TESTS)
  if (SERF_CPP_BUILD_COVERAGE)
    include (CodeCoverage)
    setup_target_for_coverage( SerfCppCoverage tests/SerfCppTests coverage )
  endif (SERF_CPP_BUILD_COVERAGE)

  add_subdirectory(tests)
endif (SERF_CPP_BUILD_TESTS)

if (SERF_CPP_BUILD_EXAMPLE)
  add_subdirectory(example)
endif (SERF_CPP_BUILD_EXAMPLE)
