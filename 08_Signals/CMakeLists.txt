cmake_minimum_required (VERSION 2.8.7)
project (linux_signals)

add_executable(sig sig.c)

add_executable(ping_pong ping_pong.c)

add_executable(sig_sync sig_sync.c)

