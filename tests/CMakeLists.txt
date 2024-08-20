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
message(STATUS "TEST: CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")
message(STATUS "TEST: CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "TEST: CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "TEST: CMAKE_CROSSCOMPILING: ${CMAKE_CROSSCOMPILING}")

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

if(WIN32 OR MSVC OR MINGW)
    add_compile_definitions(M1_STATIC)
    message(STATUS "Adding windows OS flags")
endif()

#----------------
# Sources
#----------------

set(M1SDK_SOURCE_PATH ../libmach1spatial)

#----------------
# Mach1 Encode Test

add_executable(Mach1EncodeTests Mach1EncodeTests/mach1encode-tests.cpp ${M1SDK_SOURCE_PATH}/api_encode/include/Mach1Encode.cpp)
target_link_libraries(Mach1EncodeTests PRIVATE M1Encode)
target_include_directories(Mach1EncodeTests PRIVATE ${M1SDK_SOURCE_PATH}/api_encode/include)

#----------------
# Mach1 Decode Test

add_executable(Mach1DecodeTests Mach1DecodeTests/mach1decode-tests.cpp ${M1SDK_SOURCE_PATH}/api_decode/include/Mach1Decode.cpp)
target_link_libraries(Mach1DecodeTests PRIVATE M1Decode)
target_include_directories(Mach1DecodeTests PRIVATE ${M1SDK_SOURCE_PATH}/api_decode/include)

#----------------
# Mach1 Decode Positional Test

add_executable(Mach1DecodePositionalTests Mach1DecodePositionalTests/mach1decodepositional-tests.cpp ${M1SDK_SOURCE_PATH}/api_decodepositional/include/Mach1DecodePositional.cpp)
target_link_libraries(Mach1DecodePositionalTests PRIVATE M1DecodePositional)
target_include_directories(Mach1DecodePositionalTests PRIVATE ${M1SDK_SOURCE_PATH}/api_decodepositional/include)

#----------------
# Mach1 Transcode Test

add_executable(Mach1TranscodeTests Mach1TranscodeTests/mach1transcode-tests.cpp ${M1SDK_SOURCE_PATH}/api_transcode/include/Mach1Transcode.cpp ${M1SDK_SOURCE_PATH}/deps/M1DSP/M1DSPUtilities.cpp ${M1SDK_SOURCE_PATH}/deps/M1DSP/M1DSPFilters.cpp)
target_link_libraries(Mach1TranscodeTests PRIVATE M1Transcode)
target_include_directories(Mach1TranscodeTests PRIVATE ${M1SDK_SOURCE_PATH}/api_transcode/include ${M1SDK_SOURCE_PATH}/deps/nlohmann/single_include)

#----------------
# Mach1 Spatial Test

add_executable(Mach1SpatialTests Mach1SpatialTests/mach1spatial-tests.cpp ${M1SDK_SOURCE_PATH}/api_encode/include/Mach1Encode.cpp ${M1SDK_SOURCE_PATH}/api_decode/include/Mach1Decode.cpp)
target_link_libraries(Mach1SpatialTests PRIVATE M1Encode M1Decode)
target_include_directories(Mach1SpatialTests PRIVATE ${M1SDK_SOURCE_PATH}/api_decode/include ${M1SDK_SOURCE_PATH}/api_encode/include)

#----------------
# Mach1 Transcode Render Test

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   if(NOT BUILD_COCOAPODS_LIBS)
      # Limiting the inclusion of this test because sndfile might have target limitations
      add_subdirectory(Mach1TranscodeRenderTest)
   endif()
endif()
#-------------------------------------------------------------------------------

include_directories(${M1SDK_SOURCE_PATH}/deps ${M1SDK_SOURCE_PATH}/deps/acutest/include)
