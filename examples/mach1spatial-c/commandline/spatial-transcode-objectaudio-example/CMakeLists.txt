cmake_minimum_required(VERSION 3.5.0)
project(spatial-transcode-objectaudio-example)
add_executable(${PROJECT_NAME})

# Use pkg-config to get hints about paths
include(FindPkgConfig)
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
mark_as_advanced(LIBSNDFILE_LIBRARY LIBSNDFILE_LIBRARIES LIBSNDFILE_INCLUDE_DIR LIBSNDFILE_INCLUDE_DIRS)
pkg_search_module(SndFile REQUIRED sndfile)

if(LIBSNDFILE_FOUND)
    set(LIBSNDFILE_LIBRARIES ${LIBSNDFILE_LIBRARY})
    set(LIBSNDFILE_INCLUDE_DIRS ${LIBSNDFILE_INCLUDE_DIR})
endif(LIBSNDFILE_FOUND)

# libbw64
add_subdirectory(libs/libbw64)
add_library(IRT::bw64 INTERFACE IMPORTED GLOBAL)
set_target_properties(IRT::bw64 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libbw64/include>)
target_include_directories(${PROJECT_NAME} PRIVATE libs/libbw64/include)

# Link with lib
target_link_libraries(${PROJECT_NAME} PRIVATE M1Transcode)
set(M1_LIB_PATH ../../../../libmach1spatial)

# collect and add source files
target_sources(${PROJECT_NAME}
    PRIVATE
    src/main.cpp
    ${M1_LIB_PATH}/deps/pugixml/src/pugixml.cpp
    ${M1_LIB_PATH}/deps/yaml/yaml/Yaml.cpp
    ${M1_LIB_PATH}/api_transcode/include/Mach1Transcode.cpp
    ${M1_LIB_PATH}/deps/M1DSP/M1DSPUtilities.cpp
    ${M1_LIB_PATH}/deps/M1DSP/M1DSPFilters.cpp
    ${M1_LIB_PATH}/api_common/include/Mach1AudioTimeline.cpp
    ${M1_LIB_PATH}/api_common/src/Mach1AudioTimelineCAPI.cpp
    ${M1_LIB_PATH}/api_common/src/Mach1AudioTimelineCore.cpp
)

# include headers
set(MACH1SPATIAL_INCLUDES ${M1_LIB_PATH}/api_transcode/include ${M1_LIB_PATH}/deps ${M1_LIB_PATH}/deps/yaml ${M1_LIB_PATH}/pugixml/src)
target_include_directories(${PROJECT_NAME} PRIVATE ${MACH1SPATIAL_INCLUDES} ${LIBSNDFILE_INCLUDE_DIRS})

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBSNDFILE_LIBRARIES})
