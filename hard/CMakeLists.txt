cmake_minimum_required(VERSION 3.22)
project(hard)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(hard
        code.cpp
        exceptions.hpp
        priority_queue.hpp
        utility.hpp)
