cmake_minimum_required(VERSION 3.8)
project(Pure_C11)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES bankers_algorithm.c)
add_executable(Pure_C11 ${SOURCE_FILES})