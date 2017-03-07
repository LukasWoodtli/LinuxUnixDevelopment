cmake_minimum_required (VERSION 2.8.7)
project (linux_procfs)

add_executable(pasname pasname.c)
add_executable(pasname2 pasname2.c)

add_executable(read_shadow read_shadow.c)
add_executable(read_shadow2 read_shadow2.c)
