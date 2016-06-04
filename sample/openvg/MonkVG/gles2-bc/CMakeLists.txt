cmake_minimum_required(VERSION 3.5)

project(libgles2bc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(serial "1.0.0")
set(soserial "1")

find_package(GLUT)
find_package(OpenGL)

include_directories(Sources/OpenGLES)

set(SOURCE_FILES
        Sources/OpenGLES/OpenGLESConfig.cpp
        Sources/OpenGLES/OpenGLESContext.cpp
        Sources/OpenGLES/OpenGLESFile.cpp
        Sources/OpenGLES/OpenGLESImplementation.cpp
        Sources/OpenGLES/OpenGLESString.cpp
        Sources/OpenGLES/OpenGLESUtil.cpp
        Sources/OpenGLES/OpenGLES11/OpenGLES11Context.cpp
        Sources/OpenGLES/OpenGLES11/OpenGLES11Implementation.cpp
        Sources/OpenGLES/OpenGLES20/Attribute.cpp
        Sources/OpenGLES/OpenGLES20/MatrixStack.cpp
        Sources/OpenGLES/OpenGLES20/OpenGLES20Context.cpp
        Sources/OpenGLES/OpenGLES20/OpenGLES20Implementation.cpp
        Sources/OpenGLES/OpenGLES20/OpenGLESState.cpp
        Sources/OpenGLES/OpenGLES20/Shader.cpp
        Sources/OpenGLES/OpenGLES20/ShaderFile.cpp
        Sources/OpenGLES/OpenGLES20/ShaderProgram.cpp
        Sources/OpenGLES/OpenGLES20/ShaderSource.cpp
        Sources/OpenGLES/OpenGLES20/Uniform.cpp)

add_library(libgles2bc ${SOURCE_FILES})
set_target_properties(libgles2bc PROPERTIES VERSION ${serial} SOVERSION ${soserial})
