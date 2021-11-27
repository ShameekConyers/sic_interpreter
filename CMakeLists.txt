cmake_minimum_required(VERSION 3.10)

project(cpplox LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

file(GLOB my_src CONFIGURE_DEPENDS
	"src/*"
)

add_executable(cpplox ${my_src})
