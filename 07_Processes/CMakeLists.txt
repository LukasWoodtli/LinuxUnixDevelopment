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

add_executable(proc_dat proc_dat.c)
add_test(proc_dat proc_dat)

add_executable(child child.c)
add_test(child child)

add_executable(child2 child2.c)
add_test(child2 child2)

add_executable(child3 child3.c)
add_test(child3 child3)

add_executable(child4 child4.c)
add_test(child4 child4)

add_executable(prio_child prio_child.c)

add_executable(waise waise.c)
add_test(waise waise)

add_executable(zombie zombie.c)

add_executable(wait wait.c)

add_executable(waitpid waitpid.c)
add_test(waitpid waitpid)


add_executable(exec1 exec1.c)
add_test(exec1 exec1)

add_executable(exec2 exec2.c)
add_test(exec2 exec2)

add_executable(exec3 exec3.c)
add_test(exec3 exec3)

add_executable(exec4 exec4.c)
add_test(exec4 exec4)

add_executable(exec5 exec5.c)
add_test(exec5 exec5)

add_executable(exec6 exec6.c)
add_test(exec6 exec6)

add_executable(my_programm my_programm.c)
add_executable(exec_child exec_child.c)
add_dependencies(exec_child my_programm)

add_executable(my_find my_find.c)
add_test(my_find my_find)

add_executable(logging logging.c)

add_executable(daemon daemon.c)
