set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_COMPILER clang++)

cmake_minimum_required(VERSION 3.10)

project(cpplox LANGUAGES CXX)
set(CMAKE_OSX_ARCHITECTURES x86_64)
set(CMAKE_CXX_STANDARD 20)

file(GLOB my_src CONFIGURE_DEPENDS
	"src/*"
)

add_executable(cpplox ${my_src})
