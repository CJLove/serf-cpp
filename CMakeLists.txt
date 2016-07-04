CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

project (SerfCpp)

if(NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

set (CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH}
                        ${CMAKE_CURRENT_SOURCE_DIR}/CMake)


find_package (GTest)

if (GTEST_FOUND)
    OPTION (SERF_CPP_BUILD_TESTS "Build serf-cpp tests." ON)
endif (GTEST_FOUND)

OPTION (SERF_CPP_BUILD_EXAMPLE "Build example test driver" ON)

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
