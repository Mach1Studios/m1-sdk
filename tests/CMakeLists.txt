#----------------
# Setup
#----------------

cmake_minimum_required(VERSION 3.13.0)
project(Mach1SpatialSDK_Tests)
set(CMAKE_CXX_STANDARD 11)

message(STATUS "TEST: CMAKE_VERSION=${CMAKE_VERSION}")
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: default is Release")
endif()
message(STATUS "TEST: CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")
message(STATUS "TEST: CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "TEST: CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "TEST: CMAKE_CROSSCOMPILING: ${CMAKE_CROSSCOMPILING}")

# Prepend our CMake modules directory
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
message(STATUS "TEST: CMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'")

#----------------
# Compiler flags
#----------------

set(COMMON_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} -DBOOST_FILESYSTEM_NO_DEPRECATED")
set(CMAKE_C_FLAGS ${COMMON_FLAGS})

if(APPLE)
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

message(STATUS "CMAKE_CXX_COMPILER_VERSION='${CMAKE_CXX_COMPILER_VERSION}'")
message(STATUS "CMAKE_CXX_FLAGS='${CMAKE_CXX_FLAGS}'")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG='${CMAKE_CXX_FLAGS_DEBUG}'")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE='${CMAKE_CXX_FLAGS_RELEASE}'")
message(STATUS "CMAKE_CXX_COMPILE_OBJECT='${CMAKE_CXX_COMPILE_OBJECT}'")

#----------------
# Sources
#----------------

set(M1_LIB_PATH ../libmach1spatial)

#----------------
# Mach1 Encode Test

add_executable(Mach1EncodeTests Mach1EncodeTests/mach1encode-tests.cpp ${M1_LIB_PATH}/api_encode/include/Mach1Encode.cpp)
target_link_libraries(Mach1EncodeTests M1Encode)
target_include_directories(Mach1EncodeTests PRIVATE ../libmach1spatial/api_encode/include)

#----------------
# Mach1 Decode Test

add_executable(Mach1DecodeTests Mach1DecodeTests/mach1decode-tests.cpp ${M1_LIB_PATH}/api_decode/include/Mach1Decode.cpp)
target_link_libraries(Mach1DecodeTests M1Decode)
target_include_directories(Mach1DecodeTests PRIVATE ../libmach1spatial/api_decode/include)

#----------------
# Mach1 Decode Positional Test

add_executable(Mach1DecodePositionalTests Mach1DecodePositionalTests/mach1decodepositional-tests.cpp ${M1_LIB_PATH}/api_decode/include/Mach1Decode.cpp ${M1_LIB_PATH}/api_decodepositional/include/Mach1DecodePositional.cpp)
target_link_libraries(Mach1DecodePositionalTests M1DecodePositional M1Decode)
target_include_directories(Mach1DecodePositionalTests PRIVATE ../libmach1spatial/api_decodepositional/include ../libmach1spatial/api_decode/include)

#----------------
# Mach1 Transcode Test

add_executable(Mach1TranscodeTests Mach1TranscodeTests/mach1transcode-tests.cpp ${M1_LIB_PATH}/api_transcode/include/Mach1Transcode.cpp ${M1_LIB_PATH}/deps/M1DSP/M1DSPUtilities.cpp ${M1_LIB_PATH}/deps/M1DSP/M1DSPFilters.cpp)
target_link_libraries(Mach1TranscodeTests M1Transcode M1Encode)
target_include_directories(Mach1TranscodeTests PRIVATE ../libmach1spatial/api_transcode/include ../libmach1spatial/deps/nlohmann/single_include)

#----------------
# Mach1 Spatial Test

add_executable(Mach1SpatialTests Mach1SpatialTests/mach1spatial-tests.cpp ${M1_LIB_PATH}/api_encode/include/Mach1Encode.cpp ${M1_LIB_PATH}/api_decode/include/Mach1Decode.cpp)
target_link_libraries(Mach1SpatialTests M1Encode M1Decode)
target_include_directories(Mach1SpatialTests PRIVATE ../libmach1spatial/api_decode/include ../libmach1spatial/api_encode/include)

include_directories(../libmach1spatial/deps ../libmach1spatial/deps/acutest/include)

#-------------------------------------------------------------------------------

message(STATUS "CMAKE_INSTALL_PREFIX='${CMAKE_INSTALL_PREFIX}'")