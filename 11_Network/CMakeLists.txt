cmake_minimum_required (VERSION 2.8.7)
project (linux_threads)

add_executable(client1 client1.c)

add_executable(server1 server1.c)

add_executable(addr addr.c)

add_executable(hostent hostent.c)

add_executable(servent servent.c)

add_executable(servent2 servent2.c)
add_test(servent2 servent2)
