cmake_minimum_required(VERSION 3.29)
project(Chip_8emu)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chip_8emu src/main.cpp
        src/chip8.h)
