cmake_minimum_required (VERSION 2.8)

project (SerfCppTests)

enable_testing()

include_directories( ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories( ${CMAKE_SOURCE_DIR}/include )
include_directories( ${CMAKE_SOURCE_DIR}/src )




add_executable( SerfCppTests SerfMsgStructTests.cpp )

target_link_libraries( SerfCppTests gtest gtest_main )
target_link_libraries( SerfCppTests SerfCpp )
target_link_libraries( SerfCppTests boost_thread boost_system)
add_test( SerfCppTests SerfCppTests )
