cmake_minimum_required(VERSION 3.30)
project(Http)

set(CMAKE_CXX_STANDARD 23)

add_library(common STATIC ${PROJECT_SOURCE_DIR}/common
        common/common.cpp)
target_include_directories(common PUBLIC ${PROJECT_SOURCE_DIR}/ ${PROJECT_SOURCE_DIR}/common)

add_executable(Http main.cpp)

add_executable(http-client client/http-client.cpp client/http-client.h
        common/http.h)
target_link_libraries(http-client PRIVATE common)

add_executable(http-server server/http-server.cpp server/http-server.h
        common/http.h)
target_link_libraries(http-server PRIVATE common)

