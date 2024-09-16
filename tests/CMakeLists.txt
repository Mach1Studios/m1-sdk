#----------------
# Setup
#----------------

cmake_minimum_required(VERSION 3.13.0)
project(Mach1SpatialSDK_Tests)
set(CMAKE_CXX_STANDARD 11)

message(STATUS "TEST: CMAKE_VERSION=${CMAKE_VERSION}")
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Debug")
   message(STATUS "Build type not specified: default is Debug")
endif()

list(APPEND CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(common-defaults)

if(PROJECT_IS_TOP_LEVEL)
   # include libMach1Spatial
   add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libmach1spatial ${CMAKE_CURRENT_BINARY_DIR}/libmach1spatial)
endif()

#----------------
# Sources
#----------------

set(M1SDK_SOURCE_PATH ../libmach1spatial)

#----------------
# Mach1 Encode Test

add_executable(Mach1EncodeTests Mach1EncodeTests/mach1encode-tests.cpp ${M1SDK_SOURCE_PATH}/api_encode/include/Mach1Encode.tpp)
target_link_libraries(Mach1EncodeTests PRIVATE M1Encode)
target_include_directories(Mach1EncodeTests PRIVATE ${M1SDK_SOURCE_PATH}/api_encode/include ${M1SDK_SOURCE_PATH}/api_common/include)

#----------------
# Mach1 Decode Test

add_executable(Mach1DecodeTests Mach1DecodeTests/mach1decode-tests.cpp ${M1SDK_SOURCE_PATH}/api_decode/include/Mach1Decode.tpp)
target_link_libraries(Mach1DecodeTests PRIVATE M1Decode)
target_include_directories(Mach1DecodeTests PRIVATE ${M1SDK_SOURCE_PATH}/api_decode/include ${M1SDK_SOURCE_PATH}/api_common/include)

#----------------
# Mach1 Decode Positional Test

add_executable(Mach1DecodePositionalTests Mach1DecodePositionalTests/mach1decodepositional-tests.cpp ${M1SDK_SOURCE_PATH}/api_decodepositional/include/Mach1DecodePositional.cpp)
target_link_libraries(Mach1DecodePositionalTests PRIVATE M1DecodePositional)
target_include_directories(Mach1DecodePositionalTests PRIVATE ${M1SDK_SOURCE_PATH}/api_decodepositional/include ${M1SDK_SOURCE_PATH}/api_common/include)

#----------------
# Mach1 Transcode Test

add_executable(Mach1TranscodeTests Mach1TranscodeTests/mach1transcode-tests.cpp ${M1SDK_SOURCE_PATH}/api_transcode/include/Mach1Transcode.tpp ${M1SDK_SOURCE_PATH}/deps/M1DSP/M1DSPUtilities.cpp ${M1SDK_SOURCE_PATH}/deps/M1DSP/M1DSPFilters.cpp)
target_link_libraries(Mach1TranscodeTests PRIVATE M1Transcode)
target_include_directories(Mach1TranscodeTests PRIVATE ${M1SDK_SOURCE_PATH}/api_transcode/include ${M1SDK_SOURCE_PATH}/api_common/include ${M1SDK_SOURCE_PATH}/deps/nlohmann/single_include)

#----------------

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   # Mach1 Transcode Render Test
   if(NOT BUILD_COCOAPODS_LIBS)
      # Limiting the inclusion of this test because sndfile might have target limitations
      add_subdirectory(Mach1TranscodeRenderTest)
   endif()
   # Mach1 Signal Suite Tests
   if(M1S_BUILD_SIGNAL_SUITE)
      # Include the signal suite test, ideally skipping it in most common test runs
      add_subdirectory(spatial-signal-testing)
   endif()
endif()
#-------------------------------------------------------------------------------

include_directories(${M1SDK_SOURCE_PATH}/deps ${M1SDK_SOURCE_PATH}/deps/acutest/include)
