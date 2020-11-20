cmake_minimum_required(VERSION 3.1)
project(Cinema)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -fPIC")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(http_server/include)

set(lib_files
        http_server/src/HttpResponse.cpp)
set(test_files
        tests/main.cpp
        tests/HttpResponseTests.cpp)

add_library(response_lib STATIC ${lib_files})

add_executable(g_test ${test_files})
add_executable(writer http_server/writer.cpp)

target_link_libraries(g_test response_lib)
target_link_libraries(g_test ${GTEST_LIBRARIES})
target_link_libraries(g_test pthread)

