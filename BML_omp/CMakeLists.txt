cmake_minimum_required(VERSION 2.7)
project(project CXX)

set(CMAKE_BUILD_TYPE Release)

#USE C++11 IF PRESENT
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp -std=gnu++0x")
endif()
#SET(CMAKE_CXX_FLAGS -fopenmp)

include_directories(include)
file(GLOB SOURCES source/*.cpp)

SET_SOURCE_FILES_PROPERTIES(${file} PROPERTIES COMPILE_FLAGS -O2)
add_executable(project ${SOURCES})
