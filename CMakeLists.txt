cmake_minimum_required(VERSION 3.9)
project(task C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_FLAGS "-std=c99 -ggdb3 -Wall -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS "-std=c++0x")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

