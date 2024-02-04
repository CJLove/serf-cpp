[![ci](https://github.com/CJLove/serf-cpp/actions/workflows/ci.yml/badge.svg)](https://github.com/CJLove/serf-cpp/actions/workflows/ci.yml)

# serf-cpp
C++ implementation of serf client (http://www.serfdom.io)

This is a C++ library which can be used to interact with a Serf agent
using the msgpack RPC system it supports.

The Serf RPC interface is defined here:
https://www.serfdom.io/docs/agent/rpc.html.

The Master branch builds with modern compilers supporting C++11, with
no Boost dependency.  The cpp03 branch contains the last release built
for C++03 using Boost for portable thread support.

# Status
See Commands.txt for status of individual RPC commands

# Dependencies

1. CMake used for building
2. Google Test (gtest) for unit tests
3. msgpack-c (https://github.com/msgpack/msgpack-c) 3.1.1 or later
4. g++ 4.8.5 or later or clang (C++11 required)


