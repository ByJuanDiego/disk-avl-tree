cmake_minimum_required(VERSION 3.24)
project(avl_file)

set(CMAKE_CXX_STANDARD 17)

add_executable(avl_file main.cpp avl.hpp node.hpp)
