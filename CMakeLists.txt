cmake_minimum_required(VERSION 3.29)
project(SAT C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

include_directories(include)
link_libraries(raylib m)

add_executable(
        SAT
        src/main.c
        src/main.h
        src/sat.c
        src/sat.h
        src/shape.c
        src/shape.h
)
