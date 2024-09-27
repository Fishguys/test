cmake_minimum_required(VERSION 3.18)
project(Demo)

message(STATUS "############################## Demo ##############################")
# options
option(STATIC_CRT
  "Use MultiThreaded linkage for MSVC"
  OFF
)

# append cmake config module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# C standard
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING=1)

# Determine the project architecture.
if(NOT DEFINED PROJECT_ARCH)
  if(OS_WINDOWS AND "${CMAKE_GENERATOR_PLATFORM}" STREQUAL "arm64")
    set(PROJECT_ARCH "arm64")
  elseif(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PROJECT_ARCH "x86_64")
  else()
    set(PROJECT_ARCH "x86")
  endif()
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  set(OS_WINDOWS 1)

  add_definitions(-DOS_WINDOWS=1)
  add_compile_options("/M$<IF:$<BOOL:${STATIC_CRT}>,T,D>$<$<CONFIG:Debug>:d>")
  add_link_options(/DEBUG)
endif()

message(STATUS "CMAKE_BINARY_DIR is: ${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/lib)

# Config the Demo project
# ##############################################################
# add_subdirectory(ConsoleApplication1)

message(STATUS "############################## add_subdirectory is: Example ##############################")
add_subdirectory(Example)
message(STATUS "############################## add_subdirectory is: Dll1 ##############################")
add_subdirectory(Dll1)
message(STATUS "############################## add_subdirectory is: Dll2 ##############################")
add_subdirectory(Dll2)
message(STATUS "############################## add_subdirectory is: StaticLib1 ##############################")
add_subdirectory(StaticLib1)
message(STATUS "############################## add_subdirectory is: StaticLib2 ##############################")
add_subdirectory(StaticLib2)

# set default startup project
set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "Example" )