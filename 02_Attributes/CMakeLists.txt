cmake_minimum_required (VERSION 2.8.7)
project (linux_attributes)

add_executable(my_stat my_stat.c)
add_test(my_stat my_stat)

add_executable(ugid ugid.c)
add_test(ugid ugid)

add_executable(my_link my_link.c)

add_executable(file_size file_size.c)

