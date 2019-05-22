cmake_minimum_required(VERSION 3.7)
project(blowfish)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp blowfish.h blowfish.cpp)
add_executable(blowfish ${SOURCE_FILES})