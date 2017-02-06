cmake_minimum_required (VERSION 2.8)

project (SerfCpp)

add_definitions( -std=c++11 )
if (SERF_CPP_BUILD_COVERAGE)
  add_definitions( --coverage )
endif (SERF_CPP_BUILD_COVERAGE)

find_package(MsgPackC REQUIRED)

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


install(TARGETS SerfCpp DESTINATION lib)
install(FILES ../include/serf-cpp/SerfClient.h DESTINATION include/serf-cpp)
install(FILES ../include/serf-cpp/SerfMsgStructs.h DESTINATION include/serf-cpp)
install(FILES ../include/serf-cpp/ISerfListener.h DESTINATION include/serf-cpp)
