cmake_minimum_required (VERSION 3.1)

project (SerfCppTests)

enable_testing()

# Disable clang-tidy checks for unit test code
set(CMAKE_CXX_CLANG_TIDY "")


if (SERF_CPP_BUILD_COVERAGE)
  add_definitions( --coverage )
endif (SERF_CPP_BUILD_COVERAGE)
  

include_directories( . )
include_directories( ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories( ${CMAKE_SOURCE_DIR}/include )
include_directories( ${CMAKE_SOURCE_DIR}/src )




add_executable( SerfCppTests Listeners.cpp SerfMsgStructTests.cpp SerfChannelTests.cpp SerfIoThreadTests.cpp SerfClientTests.cpp )

target_link_libraries( SerfCppTests gtest gtest_main )
target_link_libraries( SerfCppTests SerfCpp )
target_link_libraries( SerfCppTests pthread m )
if ( SERF_CPP_BUILD_COVERAGE )
  target_link_libraries( SerfCppTests gcov )
endif( SERF_CPP_BUILD_COVERAGE )

add_test( SerfCppTests SerfCppTests )

