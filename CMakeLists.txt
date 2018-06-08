cmake_minimum_required(VERSION 3.8)
project(trackable_ptr)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES trackable_ptr.h trackable_ptr_extensions.h)
add_executable(trackable_ptr ${SOURCE_FILES} main.cpp)
add_executable(vector_interaction ${SOURCE_FILES} vector_interaction.cpp)