cmake_minimum_required(VERSION 3.14)

project(siclang LANGUAGES CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OSX_ARCHITECTURES x86_64)
set(BINARY_NAME ${PROJECT_NAME})
set(TEST_NAME ${PROJECT_NAME}_test)

include(FetchContent)

FetchContent_Declare(
  gtest
  GIT_REPOSITORY "https://github.com/google/googletest"
  GIT_TAG "release-1.11.0"
)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY "https://github.com/fmtlib/fmt"
  GIT_TAG "8.0.1"
)

FetchContent_MakeAvailable(gtest fmt)

file(GLOB_RECURSE my_src CONFIGURE_DEPENDS "src/**")
file(GLOB_RECURSE test_dir CONFIGURE_DEPENDS "tests/**")

add_executable(${TEST_NAME} ${test_dir} ${my_src})
target_link_libraries(${TEST_NAME} gtest fmt)
add_test(main_test ${TEST_NAME})
