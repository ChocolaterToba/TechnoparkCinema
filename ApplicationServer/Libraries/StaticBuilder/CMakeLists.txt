cmake_minimum_required(VERSION 3.9)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_FLAGS "-std=c99 -ggdb3 -Wall -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS "-std=c++0x")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

set(LIBRARY_NAME StaticBuilder)

project(${LIBRARY_NAME})
add_library(${LIBRARY_NAME} STATIC src/main.cpp)
target_include_directories(${LIBRARY_NAME} PUBLIC include)

################################
# Testing
################################

set(TEST_SRC_FILES test/main.cpp)

# Options. Turn on with 'cmake -Dmyvarname=ON'.
#                       'cmake -DENABLE_TESTING_LIB=ON'.
option(ENABLE_TESTING_LIB "Build all tests." OFF) # Makes boolean 'test' available.

if (ENABLE_TESTING_LIB)
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

  target_link_libraries(runTests ${LIBRARY_NAME})

  gtest_discover_tests(runTests)

endif()

################################
# Coverage
################################

# option(ENABLE_COVERAGE_LIB "Run coverage" OFF)

# if (ENABLE_COVERAGE_LIB)
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
