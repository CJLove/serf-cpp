cmake_minimum_required (VERSION 2.8)

project (SerfCpp)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost COMPONENTS thread system REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

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
target_link_libraries( SerfCpp boost_thread boost_system)
