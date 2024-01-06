cmake_minimum_required(VERSION 3.18)
project(POS C)

set(CMAKE_C_STANDARD 11)

add_executable(POS main.c
        terminal_display.h
        terminal_display.c
        client.h
        client.c
        server.h
        server.c
        shared.h
        libraries.h
)
target_link_libraries(POS pthread)