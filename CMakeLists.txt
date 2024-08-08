cmake_minimum_required(VERSION 3.13.0)
project(Mach1SpatialSDK)

#----------------
# Setup & Options
#----------------

# Project & repo options
option(M1S_BUILD_TESTS           "Build libmach1spatial unit tests."                  ON)
option(M1S_BUILD_EXAMPLES        "Build libmach1spatial examples."                    ON)

# API options
option(M1ENCODE_INLINE_DECODE    "Support M1Encode with built in Decode functions"    ON)
option(M1TRANSCODE_INLINE_ENCODE "Support M1Transcode with built in Encode functions" ON)

# Lib output options
option(BUILD_SHARED_LIBS         "Build shared instead of static libraries."          OFF)
option(BUILD_COCOAPODS_LIBS      "Build libs and deploy into Cocoapods example."      OFF)
option(BUILD_JITPACK_LIBS        "Build libs and deploy into Jitpack example."        OFF)
option(BUILD_OBJC_SOURCE         "Build objc swig src files."                         OFF)
option(BUILD_WEB_JS_WASM         "Build js/wasm and deploy all web examples."         OFF)
option(BUILD_MACOS_BUNDLE        "Build shared macos bundle libraries."               OFF)
option(BUILD_WIN_MT              "Build win multithreaded static libraries."          OFF)

#----------------
# Library
#----------------

add_subdirectory(libmach1spatial)

#----------------
# Repo
#----------------

# Adds the examples and/or unit testing 
if(M1S_BUILD_EXAMPLES)
    add_subdirectory(examples/mach1spatial-c/commandline)
endif()
if(M1S_BUILD_TESTS)
    add_subdirectory(tests)
endif()

#-------------------------------------------------------------------------------