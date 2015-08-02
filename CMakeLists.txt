cmake_minimum_required(VERSION 3.2)
project(lruCache)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -O2")

set(SOURCE_FILES main.cpp)
add_executable(lruCache ${SOURCE_FILES})