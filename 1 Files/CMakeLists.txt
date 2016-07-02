cmake_minimum_required (VERSION 2.8.7)
project (linux_files)

add_executable(create_file create_file.c)

add_executable(write_file write_file.c)

add_executable(cpy_file cpy_file.c)

add_executable(backward backward.c)

add_executable(dup_fd dup_fd.c)

add_executable(play_fd play_fd.c)

