cmake_minimum_required(VERSION 2.8.7)

project(LinuxUnixDevelopment)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_COLOR_MAKEFILE   ON)

set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS} "-W -Wall -Wextra  -Werror -std=c99")

enable_testing()

add_subdirectory("02_Files")
add_subdirectory("03_Attributes")
add_subdirectory("04_ProcFs")
add_subdirectory("06_SystemFiles")

