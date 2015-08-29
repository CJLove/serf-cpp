cmake_minimum_required (VERSION 2.8)

project (SerfCppTests)

enable_testing()

include_directories( ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories( ../src )

add_executable( SerfCppTests SerfMsgStructTests.cpp )

target_link_libraries( SerfCppTests gtest gtest_main )
target_link_libraries( SerfCppTests SerfCpp )

add_test( SerfCppTests SerfCppTests )
