cmake_minimum_required (VERSION 2.8.7)
project (linux_sysfiles)

add_executable(pasname pasname.c)
add_executable(pasname2 pasname2.c)

if (NOT APPLE) # no shadow.h available on macOS
add_executable(read_shadow read_shadow.c)
add_executable(read_shadow2 read_shadow2.c)
endif (NOT APPLE)

add_executable(groupname groupname.c)

add_executable(utsname utsname.c)
add_test(utsname utsname)

