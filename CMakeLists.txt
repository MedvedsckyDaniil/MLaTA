cmake_minimum_required(VERSION 3.25)
project(MLaTA)

set(CMAKE_CXX_STANDARD 20)

add_executable(MLaTA main.cpp conclude.cpp converter.cpp converter.h conclude.h to_str.cpp to_str.h)
