cmake_minimum_required(VERSION 3.16.3)

project(LinuxShell)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE shell_src_list ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(shell ${shell_src_list})