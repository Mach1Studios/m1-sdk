cmake_minimum_required(VERSION 3.13.0)
project(Mach1SpatialSDK)

#----------------
# Setup & Options
#----------------

# Project options/defaults
option(M1S_BUILD_TESTS           "Build libmach1spatial unit tests."           ON)
option(M1S_BUILD_EXAMPLES        "Build libmach1spatial examples."             ON)
option(BUILD_SHARED_LIBS         "Build shared instead of static libraries."   OFF)
option(BUILD_MACOS_BUNDLE        "Build shared macos bundle libraries."        OFF)
option(BUILD_WIN_MT              "Build win multithreaded static libraries."   OFF)
option(SKIP_ENCODE_API           "Skip building the Mach1Encode API"           OFF)
option(SKIP_DECODE_API           "Skip building the Mach1Decode API"           OFF)
option(SKIP_DECODEPOSITIONAL_API "Skip building the Mach1DecodePositional API" OFF)
option(SKIP_TRANSCODE_API        "Skip building the Mach1Transcode API"        OFF)

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