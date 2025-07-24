cmake_minimum_required(VERSION 3.29)
project(Tomasulo_CPU)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(code
        src/main.cpp
        src/reg.cpp
        src/rs.cpp)
