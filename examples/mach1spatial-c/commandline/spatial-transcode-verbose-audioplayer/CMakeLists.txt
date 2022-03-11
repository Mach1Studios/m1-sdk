cmake_minimum_required(VERSION 3.19.3)
project(spatial-transcode-verbose-audioplayer)

set(MACH1SPATIAL_LIBS_PATH "../../../../mach1spatial-libs")

if(APPLE)
    set(MACH1SPATIAL_LIBS_UNIX_PATH ${MACH1SPATIAL_LIBS_PATH}/xcode)
else()
    set(MACH1SPATIAL_LIBS_UNIX_PATH ${MACH1SPATIAL_LIBS_PATH}/linux)
endif()

# Use pkg-config to get hints about paths
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(LIBSNDFILE_PKGCONF sndfile)
    pkg_check_modules(PC_RTAUDIO rtaudio)
endif(PKG_CONFIG_FOUND)

# - Try to find libsndfile
# Once done, this will define
#  LIBSNDFILE_FOUND - system has libsndfile
#  LIBSNDFILE_INCLUDE_DIRS - the libsndfile include directories
#  LIBSNDFILE_LIBRARIES - link these to use libsndfile

# Include dir
find_path(LIBSNDFILE_INCLUDE_DIR
        NAMES sndfile.h
        PATHS ${LIBSNDFILE_PKGCONF_INCLUDE_DIRS}
        )

# Library
find_library(LIBSNDFILE_LIBRARY
        NAMES sndfile libsndfile-1
        PATHS ${LIBSNDFILE_PKGCONF_LIBRARY_DIRS}
        )

find_package(PackageHandleStandardArgs)
find_package_handle_standard_args(LibSndFile  DEFAULT_MSG  LIBSNDFILE_LIBRARY LIBSNDFILE_INCLUDE_DIR)

if(LIBSNDFILE_FOUND)
    set(LIBSNDFILE_LIBRARIES ${LIBSNDFILE_LIBRARY})
    set(LIBSNDFILE_INCLUDE_DIRS ${LIBSNDFILE_INCLUDE_DIR})
endif(LIBSNDFILE_FOUND)

include(FindPkgConfig)

mark_as_advanced(LIBSNDFILE_LIBRARY LIBSNDFILE_LIBRARIES LIBSNDFILE_INCLUDE_DIR LIBSNDFILE_INCLUDE_DIRS)
pkg_search_module(SndFile REQUIRED sndfile)

# Try to find RtAudio
# Once done this will define
#  RTAUDIO_FOUND          - System has RtAudio
#  RTAUDIO_INCLUDE_DIR    - The RtAudio include directories
#  RTAUDIO_LIBRARIES      - The libraries needed to use RtAudio
#  RTAUDIO_DEFINITIONS    - Compiler switches required for using RtAudio
#  RTAUDIO_VERSION_STRING - the version of RtAudio found

set(RTAUDIO_DEFINITIONS ${PC_RTAUDIO_CFLAGS_OTHER})
set(RTAUDIO_VERSION_STRING ${PC_RTAUDIO_VERSION})

find_path(
  RTAUDIO_INCLUDE_DIR RtAudio.h
  HINTS ${PC_RTAUDIO_INCLUDEDIR} ${PC_RTAUDIO_INCLUDE_DIRS}
)

find_library(
  RTAUDIO_LIBRARIES NAMES rtaudio
  HINTS ${PC_RTAUDIO_LIBDIR} ${PC_RTAUDIO_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  RtAudio
  REQUIRED_VARS RTAUDIO_LIBRARIES RTAUDIO_INCLUDE_DIR
  VERSION_VAR RTAUDIO_VERSION_STRING
)

if(RTAUDIO_FOUND)
    set(RTAUDIO_LIBRARIES ${RTAUDIO_LIBRARIES})
    set(RTAUDIO_INCLUDE_DIR ${RTAUDIO_INCLUDE_DIR})
endif(RTAUDIO_FOUND)

mark_as_advanced(RTAUDIO_INCLUDE_DIR RTAUDIO_LIBRARIES)
pkg_search_module(RtAudio REQUIRED rtaudio)

# collect and add source files
set(SOURCES 
	src/main.cpp
	${MACH1SPATIAL_LIBS_PATH}/xcode/include/Mach1Decode.cpp
	${MACH1SPATIAL_LIBS_PATH}/xcode/include/Mach1Transcode.cpp
	)

# create the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# link libraries
find_library(MACH1DECODE_LIBRARY 
             NAMES Mach1DecodeCAPI libMach1DecodeCAPI libMach1DecodeCAPI.a libMach1DecodeCAPI.so libMach1DecodeCAPI.lib
             PATHS ${MACH1SPATIAL_LIBS_UNIX_PATH}/lib ${MACH1SPATIAL_LIBS_PATH}/windows-x86 ${MACH1SPATIAL_LIBS_PATH}/windows-x86_64
)
find_library(MACH1TRANSCODE_LIBRARY 
             NAMES Mach1TranscodeCAPI libMach1TranscodeCAPI libMach1TranscodeCAPI.a libMach1TranscodeCAPI.so Mach1TranscodeCAPI.lib
             PATHS ${MACH1SPATIAL_LIBS_UNIX_PATH}/lib ${MACH1SPATIAL_LIBS_PATH}/windows-x86 ${MACH1SPATIAL_LIBS_PATH}/windows-x86_64
)

# include headers
set(MACH1SPATIAL_INCLUDES ${MACH1SPATIAL_LIBS_PATH}/linux/include ${MACH1SPATIAL_LIBS_PATH}/linux/include/M1DSP ${MACH1SPATIAL_LIBS_PATH}/xcode/include ${MACH1SPATIAL_LIBS_PATH}/xcode/include/M1DSP ${MACH1SPATIAL_LIBS_PATH}/windows-x86/include ${MACH1SPATIAL_LIBS_PATH}/windows-x86/include/M1DSP)
include_directories(${MACH1SPATIAL_INCLUDES})
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
include_directories(${RTAUDIO_INCLUDE_DIR})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1DECODE_LIBRARY})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1TRANSCODE_LIBRARY})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIBSNDFILE_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${RTAUDIO_LIBRARIES})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION build)