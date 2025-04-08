cmake_minimum_required(VERSION 3.29)
project(solver)

set(CMAKE_CXX_STANDARD 20)

add_executable(solver main.cpp board.hpp
        algo_ofek.cpp)
