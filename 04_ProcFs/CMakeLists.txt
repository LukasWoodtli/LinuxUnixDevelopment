cmake_minimum_required (VERSION 2.8.7)
project (linux_procfs)

add_executable(procfs_memory procfs_memory.c)
if (NOT APPLE)
add_test(procfs_memory procfs_memory)
endif (NOT APPLE)

add_executable(myinfo myinfo.c)

add_executable(my_getid my_getid.c)
if (NOT APPLE)
add_test(my_getid my_getid)
endif (NOT APPLE)

