cmake_minimum_required(VERSION 3.8)

project(gotw)

set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set(cxx_compile_options "-g -Wall -Wextra -Wpedantic -Wconversion -Wswitch-enum -Wunreachable-code -Wwrite-strings -Wcast-align -Wshadow -Wundef -Wold-style-cast -Wshadow -Wdouble-promotion")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxx_compile_options}")

include(gtest.cmake)
find_package (Threads)

function(add_unit_tests target source)
	add_executable(${target} ${source})
	target_link_libraries(${target} PRIVATE gtest ${CMAKE_THREAD_LIBS_INIT})
endfunction()

add_subdirectory(gotw008_stack_exception)
add_subdirectory(gotw002_temp)
