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
        server/pos_sockets/active_socket.c
        server/pos_sockets/active_socket.h
        server/pos_sockets/passive_socket.c
        server/pos_sockets/passive_socket.h
        server/pos_sockets/char_buffer.c
        server/pos_sockets/char_buffer.h
        server/buffer.h
        klient/my_socket.h
        klient/my_socket.c
)
target_link_libraries(POS pthread)