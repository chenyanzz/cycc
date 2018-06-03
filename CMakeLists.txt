cmake_minimum_required(VERSION 3.9)
project(cycc)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

file(GLOB SRC src/*c src/*.cpp)
file(GLOB HEADER include/*.h)

add_executable(cycc test.cpp ${SRC} ${HEADER})