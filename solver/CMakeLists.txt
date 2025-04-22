cmake_minimum_required(VERSION 3.30.5)
project(solver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOCAL -g -Wall -Wextra -D_GLIBCXX_DEBUG -Woverflow")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=bounds,address,undefined")

add_executable(eval impls/eval_impls.cpp impls/algo_ofek.cpp)
