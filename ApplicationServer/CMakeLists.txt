cmake_minimum_required(VERSION 3.9)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_FLAGS "-std=c99 -ggdb3 -Wall -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS "-std=c++0x")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

set(PROJECT_NAME AppServer)
project(${PROJECT_NAME})

set(LIBRARIES_PATH Libraries)
add_subdirectory(${LIBRARIES_PATH}/ExampleLibrary)

add_executable(${PROJECT_NAME} src/main.cpp)
include_directories(include)
file(GLOB LIBRARIES_NAMES ExampleLibrary)

set(SRC_PATH src)

################################
# Testing
################################

set(TEST_SRC_FILES test/Test.cpp)

# Options. Turn on with 'cmake -Dmyvarname=ON'.
#                       'cmake -DENABLE_TESTING=ON'.
option(ENABLE_TESTING "Build all tests." OFF) # Makes boolean 'test' available.

if (ENABLE_TESTING)
  # Install Gtest
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.8.0
  )
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)

  include(CTest)
 
  # Link runTests with what we want to test and the GTest and DZ1 library
  add_executable(runTests ${TEST_SRC_FILES})
  target_link_libraries(runTests gtest_main pthread)

  target_link_libraries(runTests ${LIBRARIES_PATH}/${LIBRARIES_NAMES})

  gtest_discover_tests(runTests)

endif()

option(ENABLE_COVERAGE "Run coverage" OFF)

# if (ENABLE_COVERAGE)
#   set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/coverage_module)
#   include(CodeCoverage)

#   if(CMAKE_COMPILER_IS_GNUCXX)
#     target_link_libraries(runTests gcov)
#   endif()

#   append_coverage_compiler_flags()
#   setup_target_for_coverage_gcovr_html(
#     NAME coverage
#     EXECUTABLE runTests
#     DEPENDENCIES ${PROJECT_NAME} runTests
#     EXCLUDE "test/main.cpp" "build/_deps*" "src/main.c")
  
# endif()
