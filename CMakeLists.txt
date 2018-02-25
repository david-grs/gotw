cmake_minimum_required(VERSION 3.8)

project(stack)

set(CMAKE_CXX_STANDARD 17)

add_definitions("-g")

include(gtest.cmake)
add_executable(tests unit_tests.cpp)

find_package (Threads)
target_link_libraries(tests PRIVATE gtest ${CMAKE_THREAD_LIBS_INIT})

