
# if(LINUX)
#   set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#   set(BUILD_SHARED_LIBS OFF)
#   set(CMAKE_EXE_LINKER_FLAGS "-static")
# endif()

# set(CMAKE_CXX_COMPILER clang++)

cmake_minimum_required(VERSION 3.20)

project(cpplox LANGUAGES CXX)
set(CMAKE_OSX_ARCHITECTURES x86_64)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY "https://github.com/fmtlib/fmt"
  GIT_TAG "8.0.1"
)
FetchContent_MakeAvailable(fmt)

file(GLOB my_src CONFIGURE_DEPENDS
  "src/*"
)

add_executable(${PROJECT_NAME} ${my_src})
target_link_libraries(${PROJECT_NAME} fmt)
