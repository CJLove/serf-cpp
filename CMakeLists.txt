CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

project (SerfCpp)

if(NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

set (CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH}
                        ${CMAKE_CURRENT_SOURCE_DIR}/CMake)


find_package (GTest)

IF (GTEST_FOUND)
    OPTION (SERF_CPP_BUILD_TESTS "Build serf-cpp tests." ON)
ENDIF (GTEST_FOUND)

OPTION (SERF_CPP_BUILD_EXAMPLE "Build example test driver" ON)

add_subdirectory(src)

IF (SERF_CPP_BUILD_TESTS)
    add_subdirectory(tests)
ENDIF (SERF_CPP_BUILD_TESTS)
IF (SERF_CPP_BUILD_EXAMPLE)
    add_subdirectory(example)
ENDIF (SERF_CPP_BUILD_EXAMPLE)
