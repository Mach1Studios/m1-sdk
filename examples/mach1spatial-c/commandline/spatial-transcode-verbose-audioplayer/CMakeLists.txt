cmake_minimum_required(VERSION 3.19.3)
project(spatial-transcode-verbose-audioplayer)

set(MACH1SPATIAL_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../")

if(WIN32 OR MSVC)
    if (NOT "$ENV{VCPKG_PATH}" STREQUAL "")
        set(VCPKG_PATH "$ENV{VCPKG_PATH}" CACHE INTERNAL "Copied from environment variable")
        message("VCPKG_PATH is set: ${VCPKG_PATH}")
        include(${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            # 64 bits
            set(MACH1SPATIAL_LIBS_PATH ${MACH1SPATIAL_SDK_PATH}/mach1spatial-libs/vs-15-2017-x86_64)
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
            # 32 bits
            set(MACH1SPATIAL_LIBS_PATH ${MACH1SPATIAL_SDK_PATH}/mach1spatial-libs/vs-15-2017-x86)
        endif()
    else()
        message(FATAL_ERROR "unexpected: the local VCPKG_PATH environment variable is not set or is empty!")
    endif()
elseif(APPLE)
    set(MACH1SPATIAL_LIBS_PATH ${MACH1SPATIAL_SDK_PATH}/mach1spatial-libs/xcode)
elseif(UNIX AND NOT APPLE)
    set(MACH1SPATIAL_LIBS_PATH ${MACH1SPATIAL_SDK_PATH}/mach1spatial-libs/linux)
else()
    set(MACH1SPATIAL_LIBS_PATH ${MACH1SPATIAL_SDK_PATH}/mach1spatial-libs/linux)
endif()
message("MACH1SPATIAL_LIBS_PATH is set: ${MACH1SPATIAL_LIBS_PATH}")

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
	${MACH1SPATIAL_LIBS_PATH}/include/Mach1Decode.cpp
	${MACH1SPATIAL_LIBS_PATH}/include/Mach1Transcode.cpp
	)

# create the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# link Mach1Spatial libraries
if(WIN32 OR MSVC)
  # Definition added for windows to check for static lib usage
  add_definitions(-DM1_STATIC)
  # Change `PATH` to reflect `/MT` or `/MD` as needed!
  # Mach1Decode
  set(MACH1DECODE_LIBRARY_DEBUG ${MACH1SPATIAL_LIBS_PATH}/lib/Static/MD/Debug/Mach1DecodeCAPId.lib)
  set(MACH1DECODE_LIBRARY_RELEASE ${MACH1SPATIAL_LIBS_PATH}/lib/Static/MD/Release/Mach1DecodeCAPI.lib)
	set(MACH1DECODE_LIBRARY debug ${MACH1DECODE_LIBRARY_DEBUG} optimized ${MACH1DECODE_LIBRARY_RELEASE})
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1DECODE_LIBRARY})
  # Mach1Transcode
  set(MACH1TRANSCODE_LIBRARY_DEBUG ${MACH1SPATIAL_LIBS_PATH}/lib/Static/MD/Debug/Mach1TranscodeCAPId.lib)
  set(MACH1TRANSCODE_LIBRARY_RELEASE ${MACH1SPATIAL_LIBS_PATH}/lib/Static/MD/Release/Mach1TranscodeCAPI.lib)
	set(MACH1TRANSCODE_LIBRARY debug ${MACH1TRANSCODE_LIBRARY_DEBUG} optimized ${MACH1TRANSCODE_LIBRARY_RELEASE})
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1TRANSCODE_LIBRARY})
else()
  find_library(MACH1DECODE_LIBRARY
               NAMES Mach1DecodeCAPI libMach1DecodeCAPI libMach1DecodeCAPI.a libMach1DecodeCAPI.so libMach1DecodeCAPI.lib
               PATHS ${MACH1SPATIAL_LIBS_PATH}/lib
  )
	find_library(MACH1TRANSCODE_LIBRARY
		           NAMES Mach1TranscodeCAPI libMach1TranscodeCAPI
		           HINTS ${MACH1SPATIAL_LIBS_PATH}/lib
	)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1DECODE_LIBRARY})
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1TRANSCODE_LIBRARY})
endif()

# include headers
set(MACH1SPATIAL_INCLUDES ${MACH1SPATIAL_LIBS_PATH}/include)
include_directories(${MACH1SPATIAL_INCLUDES})
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
include_directories(${RTAUDIO_INCLUDE_DIR})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIBSNDFILE_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${RTAUDIO_LIBRARIES})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION build)
