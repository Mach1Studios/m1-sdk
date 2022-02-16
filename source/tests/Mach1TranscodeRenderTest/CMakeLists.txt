cmake_minimum_required(VERSION 3.19.3)
project(spatial-transcode-render)
set(CMAKE_CXX_STANDARD 11)

set(MACH1SPATIAL_SOURCE_PATH "../../")

# Use pkg-config to get hints about paths
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(LIBSNDFILE_PKGCONF sndfile)
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

# collect and add source files
set(SOURCES 
	src/main.cpp
    ${MACH1SPATIAL_SOURCE_PATH}/Mach1TranscodeCore.cpp
    ${MACH1SPATIAL_SOURCE_PATH}/Mach1Point3DCore.cpp
    ${MACH1SPATIAL_SOURCE_PATH}/Mach1GenerateCoeffs.cpp
    ${MACH1SPATIAL_SOURCE_PATH}/Mach1TranscodeCAPI.cpp
    ${MACH1SPATIAL_SOURCE_PATH}/external/cpp/Mach1Transcode.cpp
    ${MACH1SPATIAL_SOURCE_PATH}/include/M1DSP/M1DSPFilters.cpp
    ${MACH1SPATIAL_SOURCE_PATH}/include/M1DSP/M1DSPUtilities.cpp
	)

# create the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# include headers
set(MACH1SPATIAL_INCLUDES ${MACH1SPATIAL_SOURCE_PATH} ${MACH1SPATIAL_SOURCE_PATH}/external/cpp ${MACH1SPATIAL_SOURCE_PATH}/include ${MACH1SPATIAL_SOURCE_PATH}/../public)
include_directories(${MACH1SPATIAL_INCLUDES})
include_directories(${LIBSNDFILE_INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1TRANSCODE_LIBRARY})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIBSNDFILE_LIBRARIES})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION build)