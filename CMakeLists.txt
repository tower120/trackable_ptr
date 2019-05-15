cmake_minimum_required(VERSION 3.8)
project(trackable_ptr)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-Wall -Wextra -pedantic -Werror)

if (SAN STREQUAL "ASAN")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=integer -fsanitize=nullability -fno-omit-frame-pointer -g")
elseif(SAN STREQUAL "MSAN")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -fno-omit-frame-pointer -g")
endif()

include_directories(include)

# ----- TEST -------

function(_add_test TESTNAME FIRSTSOURCE)
  add_executable(${TESTNAME} ${FIRSTSOURCE} ${ARGN})
  add_test(${TESTNAME} ${TESTNAME})
endfunction(_add_test)

function(_add_failure_test TESTNAME FIRSTSOURCE)
  add_executable(${TESTNAME} ${FIRSTSOURCE} ${ARGN})

  set_target_properties(${TESTNAME} PROPERTIES
      EXCLUDE_FROM_ALL true
      EXCLUDE_FROM_DEFAULT_BUILD true)


  add_test(${TESTNAME}
      COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${TESTNAME})
  #set_property(TEST ${TESTNAME} PROPERTY WILL_FAIL TRUE)
  set_tests_properties(${TESTNAME} PROPERTIES WILL_FAIL TRUE)
  # PASS_REGULAR_EXPRESSION ? instead of WILL_FAIL
endfunction(_add_failure_test)


enable_testing()
add_subdirectory(test)