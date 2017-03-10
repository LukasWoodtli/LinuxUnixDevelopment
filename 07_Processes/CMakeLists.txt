cmake_minimum_required (VERSION 2.8.7)
project (linux_processes)

add_executable(environ1 environ1.c)
add_test(environ1 environ1)

add_executable(environ2 environ2.c)
add_test(environ2 environ2)
