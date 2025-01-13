
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(WARNING "GoogleTest should not be built in Release mode!")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Add GoogleTest
    FetchContent_Declare(
            googletest
            BASE_DIR ${CMAKE_BINARY_DIR}/googletest-src
            CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/googletest-build
            CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/googletest-install
            URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
    )
    FetchContent_MakeAvailable(googletest)

    # Enable testing
    enable_testing()


    add_test(NAME HttpTests COMMAND httptests)

    add_executable(httptests httptests.cpp
            ../common/http.h ../common/include/ContentTypes.h ../common/include/HttpBase.h)
    target_include_directories(httptests PRIVATE ${CMAKE_BINARY_DIR}/libressl-install/include)
    target_include_directories(httptests PRIVATE ${CMAKE_BINARY_DIR}/googletest-install/include)
    target_link_directories(httptests PRIVATE
            ${CMAKE_BINARY_DIR}/libressl-install/lib
            ${CMAKE_BINARY_DIR}/googletest-install/lib
    )
    target_link_libraries(httptests PRIVATE common gtest_main)
    target_link_libraries(httptests PRIVATE
            tls ssl crypto
    )
    if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(STATUS "Building for Windows 64-bit")
        target_link_libraries(httptests PRIVATE ws2_32 bcrypt)
    else ()
        message(STATUS "Not a Windows 64-bit build; skipping bcrypt linking")
    endif ()
    add_dependencies(httptests libressl)


    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Code coverage is enabled GNU|Clang.")
        # Enable coverage flags for GCC and Clang
        target_compile_options(httptests PRIVATE -g --coverage)
        target_link_options(httptests PRIVATE --coverage)
    elseif (MSVC)
        message(STATUS "Code coverage is enabled MSVC.")
        # Enable code coverage for MSVC
        target_compile_options(httptests PRIVATE /DEBUG /COVERAGE)
        target_link_options(httptests PRIVATE /DEBUG /COVERAGE)
    else ()
        message(WARNING "Code coverage is not supported for this compiler.")
    endif ()


    # Function to enable sanitizers and handle suppression files
    function(enable_sanitizers target)
        message(STATUS "Enabling sanitizers for ${target}")
        if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            # Enable sanitizers
            message(STATUS "Sanitizers are enabled for GCC|Clang ${target}")
            target_compile_options(${target} PRIVATE -fsanitize=address,undefined)
            target_link_options(${target} PRIVATE -fsanitize=address,undefined)

            # Add suppression file based on compiler version
            if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 11)
                #target_link_options(${target} PRIVATE -fsanitize-ignorelist=${CMAKE_SOURCE_DIR}/tests/suppressions.txt)
            elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 12)
                target_link_options(${target} PRIVATE -fsanitize-ignorelist=${CMAKE_SOURCE_DIR}/tests/suppressions.txt)
            else ()
                #target_link_options(${target} PRIVATE -fsanitize-ignorelist=${CMAKE_SOURCE_DIR}/tests/suppressions.txt)
            endif ()
        elseif (MSVC)
            message(STATUS "Sanitizers are enabled for MSVC ${target}")
            if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.13")
                target_compile_options(${target} PRIVATE /fsanitize=address /Zi)
                target_link_options(${target} PRIVATE /fsanitize=address /DEBUG)
                target_compile_options(googletest PRIVATE /fsanitize=address /Zi)
                target_link_options(googletest PRIVATE /fsanitize=address /DEBUG)
            else ()
                message(WARNING "Sanitizers are not supported for this version of MSVC")
            endif ()
        endif ()
        # Set environment variables for sanitizers
        set_tests_properties(HttpTests PROPERTIES
                ENVIRONMENT "ASAN_OPTIONS=suppressions=${CMAKE_SOURCE_DIR}/tests/suppressions.txt;UBSAN_OPTIONS=suppressions=${CMAKE_SOURCE_DIR}/tests/suppressions.txt"
        )
    endfunction()


    enable_sanitizers(httptests)


endif ()


