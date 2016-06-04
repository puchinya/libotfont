cmake_minimum_required(VERSION 3.5)
project(libotfont)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(serial "1.0.0")
set(soserial "1")

set(SOURCE_FILES OpenType.cpp)
add_library(libotfont ${SOURCE_FILES})
set_target_properties(libotfont PROPERTIES VERSION ${serial} SOVERSION ${soserial})
