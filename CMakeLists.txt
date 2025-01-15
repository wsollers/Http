cmake_minimum_required(VERSION 3.30)
project(Http)

set(CMAKE_CXX_STANDARD 23)

# Define an option to enable or disable security options
option(ENABLE_SECURITY "Enable security-related compiler and linker flags" ON)

# Common compiler and linker flags
if (ENABLE_SECURITY)
    # Common security flags for GCC and Clang
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fstack-protector-strong -fPIE -D_FORTIFY_SOURCE=2 -fcf-protection=full")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie -Wl,-z,relro -Wl,-z,now")
    endif ()

    # Specific flags for GCC
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        # For CET (fcf-protection)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fstack-protector-strong -fPIE -D_FORTIFY_SOURCE=2 -fcf-protection=full")

        # Uncomment the following block if CET is not supported, and you want Spectre mitigations
        # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fstack-protector-strong -fPIE -D_FORTIFY_SOURCE=2 -mindirect-branch=thunk -fstack-clash-protection")
    endif ()

    # Specific flags for Clang
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=safe-stack -fsanitize=cfi")
    endif ()

    # MSVC options
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # Compiler flags
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS /sdl /analyze")

        # Linker flags
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /guard:cf /DYNAMICBASE /NXCompat")
    endif ()

endif ()
# Print out the current compiler
message(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

set(LIBRESSL_ROOT_DIR "${CMAKE_BINARY_DIR}/_deps/libressl-src")

include(FetchContent)
include(ExternalProject)

# Detect if we are using MinGW
if (CMAKE_C_COMPILER MATCHES ".*mingw.*" OR CMAKE_CXX_COMPILER MATCHES ".*mingw.*")
    message(STATUS "Compiling with MinGW")
    # Add MinGW specific settings
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++ -fno-stack-protector")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -fno-stack-protector")
elseif (MSVC)
    message(STATUS "Compiling with MSVC")
    # Add MSVC specific settings
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MT /GS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT /GS")
else ()
    message(STATUS "Compiling with an unknown compiler")
endif ()

if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
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

else ()
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
endif ()


#find_package(LibreSSL)

add_library(common STATIC ${PROJECT_SOURCE_DIR}/common
        common/common.cpp)

target_include_directories(common PUBLIC ${PROJECT_SOURCE_DIR}/ ${PROJECT_SOURCE_DIR}/common)

add_executable(Http main.cpp)


add_executable(http-client client/http-client.cpp client/http-client.h
        client/http-client-win64.h client/http-client-linux.h
        common/http.cpp
        common/http.h common/include/ContentTypes.h common/include/HttpBase.h)
target_include_directories(http-client PRIVATE ${CMAKE_BINARY_DIR}/libressl-install/include)
target_link_directories(http-client PRIVATE
        ${CMAKE_BINARY_DIR}/libressl-install/lib
)
target_link_libraries(http-client PRIVATE common)
target_link_libraries(http-client PRIVATE
        tls ssl crypto
)
if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Building for Windows 64-bit")
    target_link_libraries(http-client PRIVATE ws2_32 bcrypt)
else ()
    message(STATUS "Not a Windows 64-bit build; skipping bcrypt linking")
endif ()
add_dependencies(http-client libressl)


add_executable(http-server server/http-server.cpp server/http-server.h
        common/http.cpp
        common/http.h common/include/ContentTypes.h common/include/HttpBase.h
)
target_include_directories(http-server PRIVATE ${CMAKE_BINARY_DIR}/libressl-install/include)
target_link_directories(http-server PRIVATE
        ${CMAKE_BINARY_DIR}/libressl-install/lib
)
target_link_libraries(http-server PRIVATE common)
target_link_libraries(http-server PRIVATE
        tls ssl crypto
)


if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Building for Windows 64-bit")
    target_link_libraries(http-server PRIVATE ws2_32 bcrypt)
else ()
    message(STATUS "Not a Windows 64-bit build; skipping bcrypt linking")
endif ()
add_dependencies(http-server libressl)


option(ENABLE_TESTS "Enable building of tests" ON)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()
#cmake -DENABLE_TESTS=OFF ..




