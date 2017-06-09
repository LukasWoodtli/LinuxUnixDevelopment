cmake_minimum_required (VERSION 2.8.7)
project (linux_threads)

add_executable(thread1 thread1.c)
add_test(thread1 thread1)

add_executable(thread2 thread2.c)
add_test(thread2 thread2)

add_executable(thread3 thread3.c)
add_test(thread3 thread3)

add_executable(thread4 thread4.c)
add_test(thread4 thread4)

add_executable(thread5 thread5.c)

add_executable(thread6 thread6.c)

add_executable(thread8 thread8.c)

add_executable(thread9 thread9.c)
add_test(thread9 thread9)

add_executable(thread10 thread10.c)
add_test(thread10 thread10)

add_executable(thread11 thread11.c)
add_test(thread11 thread11)

add_executable(thread12 thread12.c)
add_test(thread12 thread12)

