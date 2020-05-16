cmake_minimum_required(VERSION 3.15)
project(DS_lab1)

set(CMAKE_CXX_STANDARD 14)

add_executable(DS_lab1 main.cpp Huffman.cpp Huffman.h compress.cpp compress.h uncompress.cpp uncompress.h)