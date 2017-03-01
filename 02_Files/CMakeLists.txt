cmake_minimum_required (VERSION 2.8.7)
project (linux_files)

add_executable(create_file create_file.c)

add_executable(write_file write_file.c)

add_executable(cpy_file cpy_file.c)

add_executable(backward backward.c)

add_executable(dup_fd dup_fd.c)
add_test(dup_fd dup_fd)

add_executable(play_fd play_fd.c)

add_executable(trash trash.c)
add_test(trash trash)

add_executable(sperre sperre.c)

add_executable(poll_stdin_time poll_stdin_time.c)
add_test(poll_stdin_time poll_stdin_time)

add_executable(write_vec write_vec.c)

add_executable(mymkdir mymkdir.c)

add_executable(mychdir mychdir.c)

add_executable(list_wd list_wd.c)
add_test(list_wd list_wd)

add_executable(scan_dir scan_dir.c)
add_test(scan_dir scan_dir)

add_executable(find_dir find_dir.c)
add_test(find_dir find_dir)

add_executable(ftwalk ftwalk.c)
add_test(ftwalk ftwalk)

