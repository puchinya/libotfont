cmake_minimum_required(VERSION 3.5)

add_subdirectory(gles2-bc)

project(libMonkVG)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(serial "1.0.0")
set(soserial "1")

include_directories(${libMonkVG_SOURCE_DIR}/include)
include_directories(${libMonkVG_SOURCE_DIR}/src)
include_directories(${libMonkVG_SOURCE_DIR}/src/opengl)
include_directories(${libMonkVG_SOURCE_DIR}/gles2-bc/Sources/OpenGLES)
include_directories(${libMonkVG_SOURCE_DIR}/gles2-bc/Sources/OpenGLES/OpenGLES11)
include_directories(${libMonkVG_SOURCE_DIR}/gles2-bc/Sources/OpenGLES/OpenGLES20)

find_package(GLUT)
find_package(OpenGL)

set(SOURCE_FILES src/mkBaseObject.cpp
        src/mkBatch.cpp src/mkContext.cpp src/mkFont.cpp src/mkImage.cpp
        src/mkMath.cpp src/mkPaint.cpp src/mkParameter.cpp src/mkPaint.cpp src/mkPath.cpp src/mkVGU.cpp
        src/opengl/glBatch.cpp src/opengl/glContext.cpp src/opengl/glFont.cpp
        src/opengl/glImage.cpp src/opengl/glPaint.cpp src/opengl/glPath.cpp)

add_library(libMonkVG ${SOURCE_FILES})
set_target_properties(libMonkVG PROPERTIES VERSION ${serial} SOVERSION ${soserial})
target_link_libraries(libMonkVG libgles2bc)
