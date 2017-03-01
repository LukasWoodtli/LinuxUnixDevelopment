cmake_minimum_required (VERSION 2.8.7)
project (linux_procfs)

add_executable(procfs_memory procfs_memory.c)
add_test(procfs_memory procfs_memory)
