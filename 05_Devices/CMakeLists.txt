cmake_minimum_required (VERSION 2.8.7)
project (linux_devices)

if(UNIX AND NOT APPLE) # linux
add_executable(cdrom cdrom.c)
endif(UNIX AND NOT APPLE)

