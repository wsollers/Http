cmake_minimum_required(VERSION 3.30)
project(Http)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

set(LIBRESSL_ROOT_DIR "${CMAKE_BINARY_DIR}/_deps/libressl-src")

include(FetchContent)
include(ExternalProject)



if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(STATUS "Compiling for Windows 64-bit")
  
  ExternalProject_Add(
      libressl
      PREFIX ${CMAKE_BINARY_DIR}/libressl
      URL https://github.com/libressl/portable/releases/download/v4.0.0/libressl_v4.0.0_windows_x64.zip
      #URL_HASH SHA256=<your_hash_here> # Optional: Add a valid hash for verification
      SOURCE_DIR ${CMAKE_BINARY_DIR}/libressl-src
      BINARY_DIR ${CMAKE_BINARY_DIR}/libressl-build
      CONFIGURE_COMMAND "" # Not needed for precompiled binaries
      BUILD_COMMAND ""     # No build step since it's a precompiled binary
      INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory 
          ${CMAKE_BINARY_DIR}/libressl-src 
          ${CMAKE_BINARY_DIR}/libressl-install
  )

else()
  message(STATUS "Compiling for Linux 64-bit")

  ExternalProject_Add(
      libressl
      PREFIX ${CMAKE_BINARY_DIR}/libressl
      URL https://github.com/libressl/portable/releases/download/v4.0.0/libressl-4.0.0.tar.gz
      #URL_HASH SHA256=<your_hash_here> # Optional: Add a valid hash for verification
      SOURCE_DIR ${CMAKE_BINARY_DIR}/libressl-src
      BINARY_DIR ${CMAKE_BINARY_DIR}/libressl-build
      CMAKE_ARGS
          -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/libressl-install
          -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  )
endif()


#find_package(LibreSSL)

add_library(common STATIC ${PROJECT_SOURCE_DIR}/common
        common/common.cpp)

target_include_directories(common PUBLIC ${PROJECT_SOURCE_DIR}/ ${PROJECT_SOURCE_DIR}/common)

add_executable(Http main.cpp)


add_executable(http-client client/http-client.cpp client/http-client.h
  client/http-client-win64.h client/http-client-linux.h
  common/http.h common/include/ContentTypes.h common/include/HttpBase.h)
target_include_directories(http-client PRIVATE ${CMAKE_BINARY_DIR}/libressl-install/include)
target_link_directories(http-client PRIVATE
      ${CMAKE_BINARY_DIR}/libressl-install/lib
)
target_link_libraries(http-client PRIVATE common)
target_link_libraries(http-client PRIVATE 
  tls ssl crypto
)
if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Building for Windows 64-bit")
    target_link_libraries(http-client PRIVATE ws2_32 bcrypt)
else()
    message(STATUS "Not a Windows 64-bit build; skipping bcrypt linking")
endif()
add_dependencies(http-client libressl)








add_executable(http-server server/http-server.cpp server/http-server.h
    common/http.h common/include/ContentTypes.h common/include/HttpBase.h)
target_include_directories(http-server PRIVATE ${CMAKE_BINARY_DIR}/libressl-install/include)
target_link_directories(http-server PRIVATE
      ${CMAKE_BINARY_DIR}/libressl-install/lib
)
target_link_libraries(http-server PRIVATE common)
target_link_libraries(http-server PRIVATE 
  tls ssl crypto
)
if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Building for Windows 64-bit")
    target_link_libraries(http-server PRIVATE ws2_32 bcrypt)
else()
    message(STATUS "Not a Windows 64-bit build; skipping bcrypt linking")
endif()
add_dependencies(http-server libressl)



option(ENABLE_TESTS "Enable building of tests" ON)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
#cmake -DENABLE_TESTS=OFF ..











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
