cmake_minimum_required (VERSION 2.8)

project (SerfCppExample)


include_directories( ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories( ${CMAKE_SOURCE_DIR}/include )
include_directories( ${CMAKE_SOURCE_DIR}/src )

add_executable( SerfCppExample SerfCppExample.cpp )

target_link_libraries( SerfCppExample SerfCpp )
target_link_libraries( SerfCppExample boost_thread boost_system)
