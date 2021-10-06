cmake_minimum_required(VERSION 3.20)
project(tcp_client_server C)

set(CMAKE_C_STANDARD 99)

add_executable(tcp_client client.c)
add_executable(tcp_server server.c)
