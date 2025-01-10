cmake_minimum_required(VERSION 3.30)
project(Http)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

set(LIBRESSL_ROOT_DIR "${CMAKE_BINARY_DIR}/_deps/libressl-src")

set(ARCHITECTURE "x64") #ARM64 Win32

if(WIN32)
    set(OS_NAME "windows")
elseif(UNIX)
    if(APPLE)
    else()
        set(OS_NAME "linux")
    endif()
endif()




include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
)
FetchContent_MakeAvailable(googletest)

 #   add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

set(LIBRESSL_VERSIONM "4.0.0")
if(WIN32)
    FetchContent_Declare(
            libressl
            URL https://github.com/libressl/portable/releases/download/v4.0.0/libressl_v4.0.0_${OS_NAME}_${ARCHITECTURE}.zip
    )
else()
    FetchContent_Declare(
            libressl
            URL https://github.com/libressl/portable/releases/download/v4.0.0/libressl-4.0.0.tar.gz
    )
endif()


FetchContent_MakeAvailable(libressl)


add_library(common STATIC ${PROJECT_SOURCE_DIR}/common
        common/common.cpp)
target_include_directories(common PUBLIC ${PROJECT_SOURCE_DIR}/ ${PROJECT_SOURCE_DIR}/common)

add_executable(Http main.cpp)




#add_dependencies(Http Build_LibreSSL)

add_executable(http-client client/http-client.cpp client/http-client.h
        common/http.h)
find_package(LibreSSL REQUIRED)
target_link_libraries(http-client PRIVATE common LibreSSL::TLS libcmt)

add_executable(http-server server/http-server.cpp server/http-server.h
        common/http.h)
find_package(LibreSSL REQUIRED)
target_link_libraries(http-server PRIVATE common LibreSSL::TLS libcmt)

