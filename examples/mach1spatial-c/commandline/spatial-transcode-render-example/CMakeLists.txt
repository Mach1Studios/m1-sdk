cmake_minimum_required(VERSION 3.5.0)
project(spatial-transcode-render)
add_executable(${PROJECT_NAME})

if(PROJECT_IS_TOP_LEVEL)
    list(APPEND CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/../../../../cmake")
    include(common-defaults)

    # include libMach1Spatial
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../../libmach1spatial ${CMAKE_CURRENT_BINARY_DIR}/libmach1spatial)
endif()

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

# Link with lib
target_link_libraries(${PROJECT_NAME} PRIVATE M1Transcode)
set(M1_LIB_PATH ../../../../libmach1spatial)

# collect and add source files
target_sources(${PROJECT_NAME}
PRIVATE
	src/main.cpp
    ${M1_LIB_PATH}/deps/M1DSP/M1DSPUtilities.cpp
    ${M1_LIB_PATH}/deps/M1DSP/M1DSPFilters.cpp
)

# include headers
set(MACH1SPATIAL_INCLUDES ${M1_LIB_PATH}/api_transcode/include ${M1_LIB_PATH}/deps)
target_include_directories(${PROJECT_NAME} PRIVATE ${MACH1SPATIAL_INCLUDES} ${LIBSNDFILE_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBSNDFILE_LIBRARIES})
