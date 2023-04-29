cmake_minimum_required(VERSION 3.24)
project(avl_file)

set(CMAKE_CXX_STANDARD 17)

add_executable(read_data src/read_bin_file.cpp inc/node.hpp src/read_bin_file.cpp inc/record.hpp)

add_executable(index_by_end_year src/index_by_end_year.cpp)
add_executable(index_by_votes src/index_by_votes.cpp)
add_executable(index_by_rating src/index_by_rating.cpp)
add_executable(index_by_gross src/index_by_gross.cpp)
add_executable(index_by_data_id src/index_by_data_id.cpp)
add_executable(index_by_length src/index_by_length.cpp)
add_executable(index_by_certificate src/index_by_certificate.cpp)
add_executable(index_by_release_year src/index_by_release_year.cpp)
add_executable(index_by_content_type src/index_by_content_type.cpp)
add_executable(index_by_title src/index_by_title.cpp src/index_by_title.cpp)
