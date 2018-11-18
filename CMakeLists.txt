cmake_minimum_required(VERSION 3.12)
project(Pure_C11)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")
set(SOURCE_FILES main.c)
add_executable(Pure_C11 ${SOURCE_FILES})