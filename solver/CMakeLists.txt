cmake_minimum_required(VERSION 4.0)
project(solver)

set(CMAKE_CXX_STANDARD 20)

add_executable(solver main.cpp board.hpp
        algo_ofek.cpp
        monte_carlo_tree_simple.cpp)
