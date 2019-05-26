cmake_minimum_required (VERSION 3.1)

project (SerfCpp)

if (SERF_CPP_BUILD_COVERAGE)
  add_definitions( --coverage )
endif (SERF_CPP_BUILD_COVERAGE)


include_directories( ${CMAKE_SOURCE_DIR}/include )
include_directories( . )

set (SerfCpp_SRCS
    ISerfListener.cpp
    SerfMsgStructs.cpp
    SerfClient.cpp
    SerfChannel.cpp
    SerfIoThread.cpp
)


add_library (SerfCpp SHARED ${SerfCpp_SRCS})
if (SERF_CPP_BUILD_COVERAGE)
  target_link_libraries( SerfCpp gcov )
endif (SERF_CPP_BUILD_COVERAGE)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "asan" )
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
    target_link_libraries( SerfCpp asan )
  endif ()
endif ()
if ("${CMAKE_BUILD_TYPE}" STREQUAL "tsan" )
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" ) 
    target_link_libraries( SerfCpp tsan )
  endif ()
endif ()
if ("${CMAKE_BUILD_TYPE}" STREQUAL "lsan" )
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
    target_link_libraries( SerfCpp lsan )
  endif ()
endif ()
if ("${CMAKE_BUILD_TYPE}" STREQUAL "ubsan" )
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
    target_link_libraries( SerfCpp ubsan )
  endif ()
endif ()

install(TARGETS SerfCpp DESTINATION lib)
install(FILES ../include/serf-cpp/SerfClient.h DESTINATION include/serf-cpp)
install(FILES ../include/serf-cpp/SerfMsgStructs.h DESTINATION include/serf-cpp)
install(FILES ../include/serf-cpp/ISerfListener.h DESTINATION include/serf-cpp)
