cmake_minimum_required (VERSION 2.8.7)
project (linux_terminal)

add_executable(my_eof my_eof.c)

add_executable(my_getpass my_getpass.c)

add_executable(keystroke keystroke.c)

add_executable(my_getch my_getch.c)

add_executable(my_tty my_tty.c)
add_test(my_tty my_tty)

add_executable(sende sende.c)

