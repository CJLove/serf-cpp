cmake_minimum_required (VERSION 2.8)

project (SerfClientExample)


include_directories( ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories( ${CMAKE_SOURCE_DIR}/include )
include_directories( ${CMAKE_SOURCE_DIR}/src )

add_executable( SerfClientExample SerfClientExample.cpp )

target_link_libraries( SerfClientExample SerfCpp )
target_link_libraries( SerfClientExample boost_thread boost_system)
