cmake_minimum_required(VERSION 2.8.7)

project(LinuxUnixDevelopment)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_COLOR_MAKEFILE   ON)

if(UNIX AND NOT APPLE)
  set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-W -Wall -Wextra  -Werror -std=c99 -pthread")
else()
  set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-W -Wall -Wextra  -Werror -std=c99")
endif()


if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE DEBUG)
endif()


enable_testing()

add_subdirectory("02_Files")
add_subdirectory("03_Attributes")
add_subdirectory("04_ProcFs")
add_subdirectory("05_Devices")
add_subdirectory("06_SystemFiles")
add_subdirectory("07_Processes")
add_subdirectory("08_Signals")
add_subdirectory("09_IPC")
add_subdirectory("10_Threads")
add_subdirectory("11_Network")
add_subdirectory("13_Terminal")
