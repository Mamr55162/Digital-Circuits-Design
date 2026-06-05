cmake_minimum_required(VERSION 4.2)
project(Digital_Circuits_Design)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(Digital_Circuits_Design
        include/combinational_circuits.h
        src/combinational_circuits.cpp)
