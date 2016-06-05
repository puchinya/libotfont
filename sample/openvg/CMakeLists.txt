cmake_minimum_required(VERSION 3.5)

add_subdirectory(MonkVG)

project(vgsample)

include_directories(MonkVG/include)
include_directories(../../src)

find_package(GLUT)
find_package(OpenGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(serial "1.0.0")

set(SOURCE_FILES main.cpp file_source.c malloc_heap.c)
add_executable(vgsample ${SOURCE_FILES})
set_target_properties(vgsample PROPERTIES VERSION ${serial})
target_link_libraries(vgsample libMonkVG libotfont
        ${GLUT_LIBRARY}
        ${OPENGL_LIBRARY})

file(INSTALL OpenSans-Regular.ttf DESTINATION ${vgsample_BINARY_DIR})
