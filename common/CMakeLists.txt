cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)

project(common)

add_library(common STATIC common.cpp http.cpp)

target_include_directories( common PUBLIC ${PROJECT_SOURCE_DIR}/include)

