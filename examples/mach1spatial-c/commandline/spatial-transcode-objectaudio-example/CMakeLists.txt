cmake_minimum_required(VERSION 3.1.0)
project(spatial-transcode-objectaudio-example)
add_executable(${PROJECT_NAME})

# Use pkg-config to get hints about paths
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(LIBSNDFILE_PKGCONF sndfile)
    pkg_check_modules(LIBBW64_PKGCONF bw64)
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
add_subdirectory(libs/libbw64)

mark_as_advanced(LIBSNDFILE_LIBRARY LIBSNDFILE_LIBRARIES LIBSNDFILE_INCLUDE_DIR LIBSNDFILE_INCLUDE_DIRS)
pkg_search_module(SndFile REQUIRED sndfile)

# collect and add source files
target_sources(${PROJECT_NAME}
    PRIVATE
    src/main.cpp
    ../../../../libmach1spatial/deps/pugixml/src/pugixml.cpp
    ../../../../libmach1spatial/deps/yaml/yaml/Yaml.cpp
    ../../../../libmach1spatial/include/cpp/Mach1Transcode.cpp
    ../../../../libmach1spatial/include/cpp/Mach1AudioTimeline.cpp
)

# include headers
set(MACH1SPATIAL_INCLUDES ../../../../libmach1spatial/include/cpp ../../../../libmach1spatial/deps)
include_directories(${MACH1SPATIAL_INCLUDES} ${LIBSNDFILE_INCLUDE_DIRS})

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBSNDFILE_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE bw64)

install(TARGETS ${PROJECT_NAME} DESTINATION build)
