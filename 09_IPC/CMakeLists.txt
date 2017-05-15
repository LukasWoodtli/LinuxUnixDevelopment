cmake_minimum_required (VERSION 2.8.7)
project (linux_ipc)

add_executable(pipe1 pipe1.c)

add_executable(pipe2 pipe2.c)

add_executable(pipe3 pipe3.c)

add_executable(filter filter.c)

add_executable(pipe3b pipe3b.c)
add_dependencies(pipe3b filter)

add_executable(popen1 popen1.c)

add_executable(popen2 popen2.c)

add_executable(printme printme.c)

add_executable(fifo2 fifo2.c)
add_test(fifo2 fifo2)

