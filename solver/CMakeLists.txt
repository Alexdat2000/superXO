cmake_minimum_required(VERSION 4.0)
project(solver)

set(CMAKE_CXX_STANDARD 20)

add_executable(solver main.cpp)
add_definitions(-DCROW_USE_BOOST)
