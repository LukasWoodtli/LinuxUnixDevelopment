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

if(UNIX AND NOT APPLE) # unnamed semaphores are deprecated in macOS
add_executable(thread13 thread13.c)
add_test(thread13 thread13)
endif()

add_executable(thread14 thread14.c)
add_test(thread14 thread14)

add_executable(thread15 thread15.c)
add_test(thread15 thread15)

add_executable(thread17 thread17.c)
add_test(thread17 thread17)

add_executable(thread18 thread18.c)
add_test(thread18 thread18)

add_executable(thread19 thread19.c)
add_test(thread19 thread19)

add_executable(thread20 thread20.c)
add_test(thread20 thread20)

