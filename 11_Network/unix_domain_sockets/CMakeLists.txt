cmake_minimum_required (VERSION 2.8.7)
project (linux_network)

add_executable(uds_server uds_server.c)

add_executable(uds_client uds_client.c)

add_executable(fullduplex fullduplex.c)

