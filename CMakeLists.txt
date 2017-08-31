cmake_minimum_required(VERSION 3.8)
project(trackable_ptr)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp trackable_ptr2.h trackable_ptr2.cpp)
add_executable(trackable_ptr ${SOURCE_FILES})