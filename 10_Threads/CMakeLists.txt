cmake_minimum_required (VERSION 2.8.7)
project (linux_threads)

add_executable(thread1 thread1.c)
add_test(thread1 thread1)

add_executable(thread2 thread2.c)
add_test(thread2 thread2)
