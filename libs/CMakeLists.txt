cmake_minimum_required(VERSION 2.8)

project(future C CXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")  
  set(CMAKE_CXX_FLAGS "-O3 -g -Wall -std=c++14 -fexceptions")
  set(CMAKE_LINK_FLAGS "-pthreads")
endif()

if (APPLE OR WIN32)
    set(Boost_USE_STATIC_LIBS TRUE)
endif()

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0601)
endif(WIN32)

set (Boost_MULTITHREADED ON) 

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package( Threads )
if(CMAKE_USE_PTHREADS_INIT)
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
endif()

#find_package(Boost 1.62.0 REQUIRED COMPONENTS system thread)
# boost 1.62.0 cmake.FindBoost doesn't support import targets quite yet
# as a result the find_package() with required components will produce a
# warning.  I don't like warnings, so let's just leave off the required
# components for now
find_package(Boost 1.62.0 REQUIRED COMPONENTS)

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

include_directories(libraries)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE
  libraries/src/future.cpp)

set(HEADERS
  libraries/stlab/config.hpp
  libraries/stlab/channel.hpp
  libraries/stlab/expected.hpp
  libraries/stlab/future.hpp
  libraries/stlab/progress.hpp)

add_library(future ${SOURCE} ${HEADERS})
set_property(TARGET future PROPERTY CXX_STANDARD 14)
set_property(TARGET future PROPERTY CXX_STANDARD_REQUIRED ON)
