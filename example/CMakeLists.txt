cmake_minimum_required (VERSION 3.15)

project (SerfClientExample)

 include_directories( ${CMAKE_SOURCE_DIR}/include )

add_executable( SerfClientExample SerfClientExample.cpp )

target_link_libraries( SerfClientExample SerfCpp )
target_link_libraries( SerfClientExample pthread )
