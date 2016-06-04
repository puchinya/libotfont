cmake_minimum_required(VERSION 3.5)

set(REPO ~/ClionProjects/.repo)

project(unittest)

include(CTest)
enable_testing()

include(${REPO}/DownloadProject/DownloadProject.cmake)

download_project(
        PROJ                googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             master
        UPDATE_DISCONNECTED 1
)

include_directories(../src)
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(test_SRCS byte_array.c test_byte_array.cpp test_OpenType.cpp)
add_executable(runUnitTests gtest.cpp ${test_SRCS})
target_link_libraries(runUnitTests gtest gmock libotfont)

