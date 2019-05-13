cmake_minimum_required(VERSION 3.8)
project(trackable_ptr)

set(CMAKE_CXX_STANDARD 17)


function(_add_test TESTNAME FIRSTSOURCE)
  add_executable(${TESTNAME} ${FIRSTSOURCE} ${ARGN})
  add_test(${TESTNAME} ${TESTNAME})
endfunction(_add_test)



include_directories(include)


#option(BUILD_TESTING "" OFF)

enable_testing()
#include(CTest)


#add_subdirectory(exmpale)
add_subdirectory(test)

#set(SOURCE_FILES include/tower120/trackable_ptr.h include/tower120/trackable_ptr_extensions.h)
#add_executable(trackable_ptr ${SOURCE_FILES} example/main.cpp)
#add_executable(vector_interaction ${SOURCE_FILES} example/vector_interaction.cpp)