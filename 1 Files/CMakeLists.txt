cmake_minimum_required (VERSION 2.8.7)
project (linux_files)

add_executable(create_file create_file.c)

add_executable(write_file write_file.c)

add_executable(cpy_file cpy_file.c)

add_executable(backward backward.c)

add_executable(dup_fd dup_fd.c)

add_executable(play_fd play_fd.c)

add_executable(trash trash.c)

add_executable(sperre sperre.c)

add_executable(poll_stdin_time poll_stdin_time.c)

add_executable(write_vec write_vec.c)

add_executable(mymkdir mymkdir.c)

add_executable(mychdir mychdir.c)

