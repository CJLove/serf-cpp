cmake_minimum_required (VERSION 2.8)

project (SerfCpp)

find_package(MsgPackC REQUIRED)

set (SerfCpp_SRCS
    SerfMsgStructs.cpp
)

add_library (SerfCpp SHARED ${SerfCpp_SRCS})
