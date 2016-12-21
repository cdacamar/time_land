cmake_minimum_required(VERSION 2.8)

project(time_land)

include_directories(./)
include_directories(./libs/libraries/)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  set(DEBUG_FLAGS "/GS /W3 /Gm- /Zi /Od /Oy- /Ob0 /D\"_DEBUG\" /MDd")
  set(RELEASE_FLAGS "/GS /GL /W3 /Gy /Zi /O2 /MD")
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
else()
  set(DEBUG_FLAGS "-g -O0 -Wall -Werror -pedantic -std=c++1z -lpthread")
  set(RELEASE_FLAGS "-O3 -Wall -Werror -pedantic -std=c++1z -lpthread")
endif()

set(CMAKE_CXX_FLAGS_DEBUG ${DEBUG_FLAGS})
set(CMAKE_CXX_FLAGS_RELEASE ${RELEASE_FLAGS})

set(CMAKE_CONFIGURATION_TYPES Debug Release)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
  message(STATUS "Build type unspecified. Defaulting type: ${CMAKE_BUILD_TYPE}")
endif (NOT CMAKE_BUILD_TYPE)


find_package(SFML 2 COMPONENTS graphics window system)

add_subdirectory(libs)

file(GLOB_RECURSE ENTITIES_SRC RELATIVE_PATH
      entities/*.cpp
      entities/*.h)
source_group("entities" FILES ${ENTITIES_SRC})

file(GLOB_RECURSE GAME_STATES_SRC RELATIVE_PATH
      game_states/*.cpp
      game_states/*.h)
source_group("game_states" FILES ${GAME_STATES_SRC})

file(GLOB_RECURSE UTIL_SRC RELATIVE_PATH
      util/*.cpp
      util/*.h)
source_group("util" FILES ${UTIL_SRC})

set(TIMELAND_SRC main.cpp
                  ${ENTITIES_SRC}
                  ${GAME_STATES_SRC}
                  ${UTIL_SRC})

add_executable(time_land ${TIMELAND_SRC})

target_link_libraries(time_land
                      future
                      ${SFML_LIBRARIES})

set_property(TARGET time_land PROPERTY FOLDER "time_land")
