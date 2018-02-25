cmake_minimum_required(VERSION 3.8)

project(stack)

set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set(cxx_compile_options "-g -Wall -Wextra -Wpedantic -Wconversion -Wswitch-enum -Wunreachable-code -Wwrite-strings -Wcast-align -Wshadow -Wundef -Wold-style-cast -Wshadow -Wdouble-promotion")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxx_compile_options}")

include(gtest.cmake)
add_executable(tests unit_tests.cpp)

find_package (Threads)
target_link_libraries(tests PRIVATE gtest ${CMAKE_THREAD_LIBS_INIT})

