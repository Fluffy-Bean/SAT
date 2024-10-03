cmake_minimum_required(VERSION 3.29)
project(SAT C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

link_libraries(raylib m)

add_executable(
        SAT
        src/main.c
        src/main.h
)
