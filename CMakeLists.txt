cmake_minimum_required(VERSION 3.30)
project(Http)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

set(LIBRESSL_ROOT_DIR "${CMAKE_BINARY_DIR}/lib")

include(FetchContent)

if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Cross-compiling for Windows 64-bit")
  FetchContent_Declare(
        libressl
        #URL https://github.com/libressl/portable/releases/download/v4.0.0/libressl-4.0.0.tar.gz 
        #BINARY_DIR     ${CMAKE_BINARY_DIR}/custom/libressl-build       
        URL https://github.com/libressl/portable/releases/download/v4.0.0/libressl_v4.0.0_windows_x64.zip

)
else()
message(STATUS "Cross-compiling for Linux 64-bit")
FetchContent_Declare(
        libressl
        URL https://github.com/libressl/portable/releases/download/v4.0.0/libressl-4.0.0.tar.gz 
        #BINARY_DIR     ${CMAKE_BINARY_DIR}/custom/libressl-build       
        #URL https://github.com/libressl/portable/releases/download/v4.0.0/libressl_v4.0.0_windows_x64.zip

)
endif()

FetchContent_MakeAvailable(libressl)


FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
)
FetchContent_MakeAvailable(googletest)


 #   add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

find_package(LibreSSL)

add_library(common STATIC ${PROJECT_SOURCE_DIR}/common
        common/common.cpp)

target_include_directories(common PUBLIC ${PROJECT_SOURCE_DIR}/ ${PROJECT_SOURCE_DIR}/common)
target_include_directories(common PRIVATE ${CMAKE_BINARY_DIR}/include)
#add_dependencies(common Build_LibreSSL)


add_executable(Http main.cpp)


add_executable(http-client client/http-client.cpp client/http-client.h
  client/http-client-win64.h client/http-client-linux.h
  common/http.h)


target_include_directories(common PRIVATE ${CMAKE_BINARY_DIR}/include)
target_link_libraries(http-client PRIVATE common)
target_link_libraries(http-client PRIVATE tls )


add_executable(http-server server/http-server.cpp server/http-server.h
        common/http.h)
target_include_directories(common PRIVATE ${CMAKE_BINARY_DIR}/include)
target_link_libraries(http-server PRIVATE common tls)















# Output the include directories
message(STATUS "Include directories:")
get_property(INCLUDE_DIRS DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
foreach(INCLUDE_DIR ${INCLUDE_DIRS})
    message(STATUS "${INCLUDE_DIR}")
endforeach()
# Output the link directories message(STATUS "Link directories:")
get_property(LINK_DIRS TARGET Http PROPERTY LINK_DIRECTORIES)
foreach(LINK_DIR ${LINK_DIRS})
    message(STATUS)
endforeach ()
