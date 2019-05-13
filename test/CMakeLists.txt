#option(BUILD_TESTING "" OFF)

#enable_testing()
include(CTest)

_add_test(test1 test1.cpp)
_add_test(test2 test2.cpp)