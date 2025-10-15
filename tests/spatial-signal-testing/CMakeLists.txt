cmake_minimum_required(VERSION 3.13.0)
project(spatial-signal-testing)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include common cmake configuration
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
include(common-defaults)

# Include libMach1Spatial if this is built standalone
if(PROJECT_IS_TOP_LEVEL)
   add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../libmach1spatial ${CMAKE_CURRENT_BINARY_DIR}/libmach1spatial)
endif()

add_executable(${PROJECT_NAME})

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

# Set library path
set(M1_LIB_PATH ../../libmach1spatial)

# Link with lib targets (not binary files)
target_link_libraries(${PROJECT_NAME} PRIVATE M1Encode M1Decode M1Transcode)

# collect and add source files
target_sources(${PROJECT_NAME}
PRIVATE
	src/main.cpp
    src/Settings.h
    src/AudioBuffers.h
    src/AudioBuffers.cpp
    src/RtAudioChain.h
    src/RtAudioChain.cpp
    src/RtAudioLink.h
    src/GainLink.h
    src/GainLink.cpp
    src/SineWaveLink.h
    src/SineWaveLink.cpp
    src/PeakTrackerLink.h
    src/PeakTrackerLink.cpp
    src/EncodeLink.h
    src/EncodeLink.cpp
    src/DecodeLink.h
    src/DecodeLink.cpp
    src/TranscodeLink.h
    src/TranscodeLink.cpp
    src/NoiseGenLink.h
    src/NoiseGenLink.cpp
    src/NoiseGenStrategy.h

    ${M1_LIB_PATH}/deps/M1DSP/M1DSPUtilities.cpp
    ${M1_LIB_PATH}/deps/M1DSP/M1DSPFilters.cpp
)

# Create additional executable for gain test (with RtAudio)
add_executable(gain-test
    src/gain_test_main.cpp
    
    src/Settings.h
    src/AudioBuffers.h
    src/AudioBuffers.cpp
    src/RtAudioChain.h
    src/RtAudioChain.cpp
    src/RtAudioLink.h
    src/GainLink.h
    src/GainLink.cpp
    src/SineWaveLink.h
    src/SineWaveLink.cpp
    src/PeakTrackerLink.h
    src/PeakTrackerLink.cpp
    src/EncodeLink.h
    src/EncodeLink.cpp
    src/DecodeLink.h
    src/DecodeLink.cpp

    ${M1_LIB_PATH}/deps/M1DSP/M1DSPUtilities.cpp
    ${M1_LIB_PATH}/deps/M1DSP/M1DSPFilters.cpp
)

# Set C++ standard for gain test
set_property(TARGET gain-test PROPERTY CXX_STANDARD 11)
set_property(TARGET gain-test PROPERTY CXX_STANDARD_REQUIRED ON)

# Link libraries for gain test
target_link_libraries(gain-test PRIVATE M1Encode M1Decode M1Transcode)
target_include_directories(gain-test PRIVATE ${MACH1SPATIAL_INCLUDES} ${LIBSNDFILE_INCLUDE_DIRS} ${RTAUDIO_INCLUDE_DIR})
target_link_libraries(gain-test PRIVATE ${LIBSNDFILE_LIBRARIES})
target_link_libraries(gain-test PRIVATE ${RTAUDIO_LIBRARIES})

# Create offline gain test (without RtAudio dependency)
add_executable(offline-gain-test
    src/offline_gain_test.cpp
    
    src/Settings.h
    src/AudioBuffers.h
    src/AudioBuffers.cpp
    src/RtAudioLink.h
    src/SineWaveLink.h
    src/SineWaveLink.cpp
    src/EncodeLink.h
    src/EncodeLink.cpp
    src/DecodeLink.h
    src/DecodeLink.cpp

    ${M1_LIB_PATH}/deps/M1DSP/M1DSPUtilities.cpp
    ${M1_LIB_PATH}/deps/M1DSP/M1DSPFilters.cpp
)

# Set C++ standard for offline gain test
set_property(TARGET offline-gain-test PROPERTY CXX_STANDARD 11)
set_property(TARGET offline-gain-test PROPERTY CXX_STANDARD_REQUIRED ON)

# Link libraries for offline gain test (no RtAudio needed)
target_link_libraries(offline-gain-test PRIVATE M1Encode M1Decode M1Transcode)
target_include_directories(offline-gain-test PRIVATE ${MACH1SPATIAL_INCLUDES})

# Create comprehensive gain test (offline processing)
add_executable(comprehensive-gain-test
    src/comprehensive_gain_test.cpp
    
    src/Settings.h
    src/AudioBuffers.h
    src/AudioBuffers.cpp
    src/RtAudioLink.h
    src/SineWaveLink.h
    src/SineWaveLink.cpp
    src/EncodeLink.h
    src/EncodeLink.cpp
    src/DecodeLink.h
    src/DecodeLink.cpp

    ${M1_LIB_PATH}/deps/M1DSP/M1DSPUtilities.cpp
    ${M1_LIB_PATH}/deps/M1DSP/M1DSPFilters.cpp
)

# Set C++ standard for comprehensive gain test
set_property(TARGET comprehensive-gain-test PROPERTY CXX_STANDARD 11)
set_property(TARGET comprehensive-gain-test PROPERTY CXX_STANDARD_REQUIRED ON)

# Link libraries for comprehensive gain test
target_link_libraries(comprehensive-gain-test PRIVATE M1Encode M1Decode M1Transcode)
target_include_directories(comprehensive-gain-test PRIVATE ${MACH1SPATIAL_INCLUDES})

# Create quick measurement test (uses working RtAudio approach)
add_executable(quick-measurement-test
    src/quick_measurement_test.cpp
    
    src/Settings.h
    src/AudioBuffers.h
    src/AudioBuffers.cpp
    src/RtAudioChain.h
    src/RtAudioChain.cpp
    src/RtAudioLink.h
    src/GainLink.h
    src/GainLink.cpp
    src/SineWaveLink.h
    src/SineWaveLink.cpp
    src/PeakTrackerLink.h
    src/PeakTrackerLink.cpp
    src/EncodeLink.h
    src/EncodeLink.cpp
    src/DecodeLink.h
    src/DecodeLink.cpp

    ${M1_LIB_PATH}/deps/M1DSP/M1DSPUtilities.cpp
    ${M1_LIB_PATH}/deps/M1DSP/M1DSPFilters.cpp
)

# Set C++ standard for quick measurement test
set_property(TARGET quick-measurement-test PROPERTY CXX_STANDARD 11)
set_property(TARGET quick-measurement-test PROPERTY CXX_STANDARD_REQUIRED ON)

# Link libraries for quick measurement test
target_link_libraries(quick-measurement-test PRIVATE M1Encode M1Decode M1Transcode)
target_include_directories(quick-measurement-test PRIVATE ${MACH1SPATIAL_INCLUDES} ${LIBSNDFILE_INCLUDE_DIRS} ${RTAUDIO_INCLUDE_DIR})
target_link_libraries(quick-measurement-test PRIVATE ${LIBSNDFILE_LIBRARIES})
target_link_libraries(quick-measurement-test PRIVATE ${RTAUDIO_LIBRARIES})

# Create channel distribution test (no RtAudio - no segfaults)
add_executable(channel-distribution-test
    src/channel_distribution_test.cpp
    
    src/Settings.h
    src/AudioBuffers.h
    src/AudioBuffers.cpp
    src/RtAudioLink.h
    src/SineWaveLink.h
    src/SineWaveLink.cpp
    src/EncodeLink.h
    src/EncodeLink.cpp
    src/DecodeLink.h
    src/DecodeLink.cpp

    ${M1_LIB_PATH}/deps/M1DSP/M1DSPUtilities.cpp
    ${M1_LIB_PATH}/deps/M1DSP/M1DSPFilters.cpp
)

# Set C++ standard for channel distribution test
set_property(TARGET channel-distribution-test PROPERTY CXX_STANDARD 11)
set_property(TARGET channel-distribution-test PROPERTY CXX_STANDARD_REQUIRED ON)

# Link libraries for channel distribution test (no RtAudio needed)
target_link_libraries(channel-distribution-test PRIVATE M1Encode M1Decode M1Transcode)
target_include_directories(channel-distribution-test PRIVATE ${MACH1SPATIAL_INCLUDES})

# include headers
set(MACH1SPATIAL_INCLUDES ${M1_LIB_PATH}/api_common/include ${M1_LIB_PATH}/api_encode/include ${M1_LIB_PATH}/api_decode/include ${M1_LIB_PATH}/api_transcode/include ${M1_LIB_PATH}/deps)
include_directories(${MACH1SPATIAL_INCLUDES} ${LIBSNDFILE_INCLUDE_DIRS} ${RTAUDIO_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBSNDFILE_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${RTAUDIO_LIBRARIES})
