cmake_minimum_required(VERSION 3.6)
project(omgosh)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES omgosh.c omgosh.h)
add_executable(omgosh ${SOURCE_FILES})

project(prog)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES prog.c)
add_executable(prog ${SOURCE_FILES})