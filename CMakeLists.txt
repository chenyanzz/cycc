cmake_minimum_required(VERSION 3.9)
project(cycc)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(cycc main.cpp include/YCode.h include/common.h stringUtils.cpp include/stringUtils.h YType.cpp include/YType.h YNum.cpp include/YNum.h include/YException.h YExpression.cpp include/YExpression.h)