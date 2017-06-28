cmake_minimum_required (VERSION 2.8.7)
project (linux_terminal)

find_package( Curses REQUIRED )
include_directories( ${CURSES_INCLUDE_DIRS} )


add_executable(my_eof my_eof.c)

add_executable(my_getpass my_getpass.c)

add_executable(keystroke keystroke.c)

add_executable(my_getch my_getch.c)

add_executable(my_tty my_tty.c)
add_test(my_tty my_tty)

add_executable(sende sende.c)

add_executable(baud baud.c)

add_executable(setbaud setbaud.c)

add_executable(terminfo terminfo.c)
target_link_libraries(terminfo ${CURSES_LIBRARIES})
add_test(terminfo terminfo)

add_executable(terminfo2 terminfo2.c)
target_link_libraries(terminfo2 ${CURSES_LIBRARIES})
