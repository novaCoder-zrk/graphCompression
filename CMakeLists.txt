cmake_minimum_required(VERSION 3.19)
project(graphCompression)

set(CMAKE_CXX_STANDARD 14)

add_executable(graphCompression main.cpp Graph.cpp Graph.h GraphCom.cpp GraphCom.h)