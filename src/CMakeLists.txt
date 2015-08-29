cmake_minimum_required (VERSION 2.8)

project (SerfCpp)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.57.0 COMPONENTS thread system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR0 "boost not found.")    
endif(Boost_FOUND)


find_package(MsgPackC REQUIRED)

include_directories( ${CMAKE_SOURCE_DIR}/include )
include_directories( . )

set (SerfCpp_SRCS
    SerfMsgStructs.cpp
    SerfClient.cpp
    SerfIoThread.cpp
)


add_library (SerfCpp SHARED ${SerfCpp_SRCS})
