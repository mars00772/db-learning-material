cmake_minimum_required(VERSION 3.10)
project(STL)

set(CMAKE_CXX_STANDARD 11)

add_executable(STL Trie.cpp Trie.h alloc.h alloc.cpp allocator.cpp allocator.h)