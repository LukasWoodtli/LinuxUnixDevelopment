cmake_minimum_required (VERSION 2.8.7)
project (linux_processes)

add_executable(environ1 environ1.c)
add_test(environ1 environ1)

add_executable(environ2 environ2.c)
add_test(environ2 environ2)

add_executable(get_env get_env.c)

add_executable(put_env put_env.c)

add_executable(set_env set_env.c)

add_executable(unset_env unset_env.c)

add_executable(my_limit my_limit.c)
add_test(my_limit my_limit)


