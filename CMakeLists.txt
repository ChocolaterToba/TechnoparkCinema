cmake_minimum_required(VERSION 3.1)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -fPIC")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

include_directories(http_server/include)

add_library(socket STATIC http_server/socket/src/socket.cpp)
target_include_directories(socket PUBLIC
                           http_server/socket/include)

target_link_libraries(server socket)
target_link_libraries(server pthread)

add_executable(
        test http_server/src/test.cpp
        http_server/src/HttpRequest.cpp
        http_server/src/HttpResponse.cpp)
