cmake_minimum_required(VERSION 3.30)
project(Http)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

set(LIBRESSL_ROOT_DIR "${CMAKE_BINARY_DIR}/lib")

include(FetchContent)

if (MSVC)
    message(STATUS "MSVC detected")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT /GS")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /GS")
endif()
# Ensure MinGW is using the correct standard libraries
if (MINGW)
    message(STATUS "MinGW detected")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()


FetchContent_Declare(
        libressl
        URL https://github.com/libressl/portable/releases/download/v4.0.0/libressl-4.0.0.tar.gz 
        #BINARY_DIR     ${CMAKE_BINARY_DIR}/custom/libressl-build       
        #URL https://github.com/libressl/portable/releases/download/v4.0.0/libressl_v4.0.0_windows_x64.zip

)
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
        common/http.h)


target_include_directories(common PRIVATE ${CMAKE_BINARY_DIR}/include)
target_link_libraries(http-client PRIVATE common)
target_link_libraries(http-client PRIVATE tls )
# Link against the necessary MinGW libraries
if (MINGW)
    target_link_libraries(http-client PRIVATE mingw32 gcc stdc++)
elseif (MSVC)
    target_link_libraries(http-client PRIVATE libcmt msvcrt)
endif()


add_executable(http-server server/http-server.cpp server/http-server.h
        common/http.h)
target_include_directories(common PRIVATE ${CMAKE_BINARY_DIR}/include)
 target_link_libraries(http-server PRIVATE common tls)
# Link against the necessary MinGW libraries
if (MINGW)
    target_link_libraries(http-server PRIVATE mingw32 gcc stdc++)
elseif (MSVC)
    target_link_libraries(http-server PRIVATE libcmt msvcrt)
endif()















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
