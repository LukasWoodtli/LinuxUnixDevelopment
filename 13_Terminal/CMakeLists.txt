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



function(build_with_ncurses name)
	add_executable(${name} ${name}.c)
	target_link_libraries(${name} ${CURSES_LIBRARIES})
endfunction(build_with_ncurses)


build_with_ncurses(terminfo)
add_test(terminfo terminfo)

build_with_ncurses(terminfo2)

build_with_ncurses(cur2)

build_with_ncurses(cur3)

build_with_ncurses(cur4)

build_with_ncurses(cur5)

build_with_ncurses(cur6)

build_with_ncurses(cur7)

build_with_ncurses(cur8)

build_with_ncurses(cur9)

build_with_ncurses(cur10)


