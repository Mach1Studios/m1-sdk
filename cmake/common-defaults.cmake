message(STATUS "Including default compiler and linker flags")

#----------------
# Compiler flags
#----------------

if(WIN32 OR MSVC OR MINGW)
    add_compile_definitions(M1_STATIC)
    message(STATUS "Adding windows OS flags")
endif()

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif()

if(DEFINED CXX_VERSION STREQUAL 14)
    set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_STANDARD 14)
    message(STATUS "SETUP: Using C++14")
elseif(DEFINED CXX_VERSION STREQUAL 17)
    set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_STANDARD 17)
    message(STATUS "SETUP: Using C++17")
else()
    if (CMAKE_VERSION VERSION_LESS "3.1")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
            message(STATUS "SETUP: Using gnu++11")
        endif()
    else()
        if (CMAKE_SYSTEM_NAME STREQUAL "Android")
            message(STATUS "SETUP: Additional Android based c++11 flags set")
            set(CMAKE_CXX_FLAGS_INIT "-std=c++11")
            set(CMAKE_CXX_FLAGS "-std=c++11")
        else()
            set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
        endif()
            set(CMAKE_CXX_STANDARD 11)
            set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ Standard (toolchain)" FORCE)
            set(CMAKE_CXX_STANDARD_REQUIRED YES CACHE BOOL "C++ Standard required" FORCE)
            message(STATUS "SETUP: Using C++11")
    endif()
endif()
message(STATUS "SETUP: Using ${CMAKE_CXX_COMPILER}")

if(WIN32 OR MSVC OR MINGW)
    set(CMAKE_DEBUG_POSTFIX d)
    # enable MSVC_RUNTIME_LIBRARY target property
    # see https://cmake.org/cmake/help/latest/policy/CMP0091.html
    if(POLICY CMP0091)
        cmake_policy(SET CMP0091 NEW)
    endif()
endif()

if(APPLE)
    message(STATUS "SETUP: Additional MacOS flags set")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

message(STATUS "CMAKE_CXX_COMPILER_VERSION='${CMAKE_CXX_COMPILER_VERSION}'")
message(STATUS "CMAKE_CXX_FLAGS='${CMAKE_CXX_FLAGS}'")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG='${CMAKE_CXX_FLAGS_DEBUG}'")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE='${CMAKE_CXX_FLAGS_RELEASE}'")

#----------------
# Link
#----------------

if(WIN32 OR MSVC OR MINGW)
    if(BUILD_WIN_MT)
        message(STATUS "Adding /MT compile options for win targets")
        add_compile_options(
            $<$<CONFIG:>:/MT>
            $<$<CONFIG:Debug>:/MTd>
            $<$<CONFIG:Release>:/MT>
        )
        set_property(TARGET Mach1DecodeAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
        set_property(TARGET Mach1DecodePositionalAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
        set_property(TARGET Mach1EncodeAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
        set_property(TARGET Mach1TranscodeAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif()
endif()
