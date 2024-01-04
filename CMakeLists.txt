cmake_minimum_required(VERSION 3.18)
project(POS C)

set(CMAKE_C_STANDARD 11)

add_executable(POS main.c
        terminalDefinition.h)
