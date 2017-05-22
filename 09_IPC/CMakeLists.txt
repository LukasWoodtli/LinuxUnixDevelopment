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

add_executable(fifo_buf fifo_buf.c)
add_test(fifo_buf fifo_buf)

add_executable(fifo4 fifo4.c)
add_test(fifo4 fifo4)

add_executable(polling_fifo polling_fifo.c)

add_executable(sender sender.c)

add_executable(client client.c)

add_executable(server server.c)

add_executable(key_ftok key_ftok.c)
add_test(key_ftok key_ftok)

add_executable(sem sem.c)

