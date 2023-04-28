cmake_minimum_required(VERSION 3.24)
project(avl_file)

set(CMAKE_CXX_STANDARD 17)

add_executable(avl_file src/main.cpp inc/avl.hpp inc/node.hpp inc/record.hpp)
add_executable(read_data src/read_bin_file.cpp inc/node.hpp src/read_bin_file.cpp inc/record.hpp)
