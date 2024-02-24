cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(raptorq)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

add_subdirectory(raptorq)
add_subdirectory(tdfec)
add_subdirectory(tdutils)
