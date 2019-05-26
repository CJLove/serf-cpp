cmake_minimum_required(VERSION 3.11)

project(SerfCpp)

set(VERSION "0.4.0")

#---------------------------------------------------------------------------------------
# Build Types including support for sanitizers
#---------------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}
    CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel tsan asan lsan msan ubsan"
    FORCE)

# ThreadSanitizer
set(CMAKE_C_FLAGS_TSAN
    "-fsanitize=thread -g -O1"
    CACHE STRING "Flags used by the C compiler during ThreadSanitizer builds."
    FORCE)
set(CMAKE_CXX_FLAGS_TSAN
    "-fsanitize=thread -g -O1"
    CACHE STRING "Flags used by the C++ compiler during ThreadSanitizer builds."
    FORCE)

# AddressSanitize
set(CMAKE_C_FLAGS_ASAN
    "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1"
    CACHE STRING "Flags used by the C compiler during AddressSanitizer builds."
    FORCE)
set(CMAKE_CXX_FLAGS_ASAN
    "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1"
    CACHE STRING "Flags used by the C++ compiler during AddressSanitizer builds."
    FORCE)

# LeakSanitizer
set(CMAKE_C_FLAGS_LSAN
    "-fsanitize=leak -fno-omit-frame-pointer -g -O1"
    CACHE STRING "Flags used by the C compiler during LeakSanitizer builds."
    FORCE)
set(CMAKE_CXX_FLAGS_LSAN
    "-fsanitize=leak -fno-omit-frame-pointer -g -O1"
    CACHE STRING "Flags used by the C++ compiler during LeakSanitizer builds."
    FORCE)

# MemorySanitizer (clang only)
set(CMAKE_C_FLAGS_MSAN
    "-fsanitize=memory -fno-optimize-sibling-calls -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer -g -O2"
    CACHE STRING "Flags used by the C compiler during MemorySanitizer builds."
    FORCE)
set(CMAKE_CXX_FLAGS_MSAN
    "-fsanitize=memory -fno-optimize-sibling-calls -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer -g -O2"
    CACHE STRING "Flags used by the C++ compiler during MemorySanitizer builds."
    FORCE)

# UndefinedBehaviour
set(CMAKE_C_FLAGS_UBSAN
    "-fsanitize=undefined"
    CACHE STRING "Flags used by the C compiler during UndefinedBehaviourSanitizer builds."
    FORCE)
set(CMAKE_CXX_FLAGS_UBSAN
    "-fsanitize=undefined"
    CACHE STRING "Flags used by the C++ compiler during UndefinedBehaviourSanitizer builds."
    FORCE)
# MemorySanitizer only supported by clang
if ("${CMAKE_BUILD_TYPE}" STREQUAL "msan" )
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU") 
    message( FATAL_ERROR "msan not supported for g++")
  endif ()
endif ()

if(NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

#---------------------------------------------------------------------------------------
# compiler config
#---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH}
                        ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

#---------------------------------------------------------------------------------------
# Dependencies
#---------------------------------------------------------------------------------------
find_package(Threads)
find_package(GTest)
find_package(MsgPackC 3.1.0 REQUIRED)

if (GTEST_FOUND)
  option (SERF_CPP_BUILD_TESTS "Build serf-cpp tests." ON)
endif (GTEST_FOUND)

option (SERF_CPP_BUILD_EXAMPLE "Build example test driver" ON)

set(SERF_CPP_BUILD_COVERAGE false CACHE BOOL "Build for code coverage")
 
add_subdirectory(src)

if (SERF_CPP_BUILD_TESTS)
  if (SERF_CPP_BUILD_COVERAGE)
    include (CodeCoverage)
    setup_target_for_coverage( SerfCppCoverage tests/SerfCppTests coverage )
  endif (SERF_CPP_BUILD_COVERAGE)

  add_subdirectory(tests)
endif (SERF_CPP_BUILD_TESTS)

if (SERF_CPP_BUILD_EXAMPLE)
  add_subdirectory(example)
endif (SERF_CPP_BUILD_EXAMPLE)
