cmake_minimum_required(VERSION 2.8.7)

project(LinuxUnixDevelopment)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

enable_testing()

add_subdirectory("02_Files")
add_subdirectory("03_Attributes")

