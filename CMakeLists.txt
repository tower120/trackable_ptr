cmake_minimum_required(VERSION 3.8)
project(trackable_ptr)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES trackable_ptr.h )
add_executable(trackable_ptr ${SOURCE_FILES} main.cpp trackable_ptr_extensions.h unique_trackable.h)
add_executable(vector_interaction ${SOURCE_FILES} vector_interaction.cpp)
add_executable(unique_trackable ${SOURCE_FILES} unique_trackable.cpp)