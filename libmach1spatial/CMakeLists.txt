
#----------------
# Setup
#----------------

cmake_minimum_required(VERSION 3.13.0)

# add the lib CMake modules directory
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
message(STATUS "CMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'")

# Prepend the global CMake modules directory
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/../cmake)
message(STATUS "CMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'")

include(common-defaults)

#----------------
# Version
#----------------

set(Mach1SpatialLibraries_VERSION_MAJOR 4)
set(Mach1SpatialLibraries_VERSION_MINOR 0)
set(Mach1SpatialLibraries_VERSION ${Mach1SpatialLibraries_VERSION_MAJOR}.${Mach1SpatialLibraries_VERSION_MINOR})

# SET API VERSIONS
set(ENCODE_VERSION              "${Mach1SpatialLibraries_VERSION_MAJOR}.${Mach1SpatialLibraries_VERSION_MINOR}.0.0")
set(DECODE_VERSION              "${Mach1SpatialLibraries_VERSION_MAJOR}.${Mach1SpatialLibraries_VERSION_MINOR}.0.0")
set(DECODEPOSITIONAL_VERSION    "${Mach1SpatialLibraries_VERSION_MAJOR}.${Mach1SpatialLibraries_VERSION_MINOR}.0.0")
set(TRANSCODE_VERSION           "${Mach1SpatialLibraries_VERSION_MAJOR}.${Mach1SpatialLibraries_VERSION_MINOR}.0.0")

#----------------
# APIs
#----------------

project(Mach1Spatial VERSION ${Mach1SpatialLibraries_VERSION})

#----------------
# Common Files

file(GLOB M1_COMMON_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/api_common/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_common/src/*.cpp")

#----------------
# Mach1 Decode API

file(GLOB M1_DECODE_API_SRC_FILES ${M1_COMMON_SRC_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/src/*.tpp")
list(FILTER M1_DECODE_API_SRC_FILES EXCLUDE REGEX ".*Emscripten.*") # Exclude the Emscripten files
list(FILTER M1_DECODE_API_SRC_FILES EXCLUDE REGEX ".*Mach1AudioTimeline.*") # Exclude the Mach1AudioTimeline API

add_library(M1Decode STATIC ${M1_DECODE_API_SRC_FILES})
set_target_properties(M1Decode
PROPERTIES
    VERSION ${DECODE_VERSION}
    SOVERSION ${DECODE_VERSION}
)
target_include_directories(M1Decode PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/api_common/include" "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/include")

# Set include paths for other APIs
set(M1DECODE_API_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/src" "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/include")

#----------------
# Mach1 Encode API
# Note: (Optional) Some functions in Mach1Encode API may optionally require Mach1Decode API

file(GLOB M1_ENCODE_API_SRC_FILES ${M1_COMMON_SRC_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/src/*.tpp")
list(FILTER M1_ENCODE_API_SRC_FILES EXCLUDE REGEX ".*Emscripten.*") # Exclude the Emscripten files
list(FILTER M1_ENCODE_API_SRC_FILES EXCLUDE REGEX ".*Mach1AudioTimeline.*") # Exclude the Mach1AudioTimeline API

add_library(M1Encode STATIC ${M1_ENCODE_API_SRC_FILES})
set_target_properties(M1Encode
PROPERTIES
    VERSION ${ENCODE_VERSION}
    SOVERSION ${ENCODE_VERSION}
)
target_include_directories(M1Encode PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/api_common/include" "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/include")

# Supports inline decode functions in M1Encode API
# (Requires linking decode api)
if(M1ENCODE_INLINE_DECODE)
    message(STATUS "ENCODE API: Supports inline decoder")
    target_include_directories(M1Encode PUBLIC ${M1DECODE_API_INCLUDE_PATHS})
else()
    message(STATUS "ENCODE API: Does not support inline decoding")
endif()

# Set include paths for other APIs
set(M1ENCODE_API_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/src" "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/include")

#----------------
# Mach1 Decode Positional API
# Note: Mach1DecodePositional API requires Mach1Decode API

file(GLOB M1_DECODEPOSITIONAL_API_SRC_FILES ${M1_COMMON_SRC_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/api_decodepositional/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_decodepositional/src/*.cpp")
list(FILTER M1_DECODEPOSITIONAL_API_SRC_FILES EXCLUDE REGEX ".*Emscripten.*") # Exclude the Emscripten files
list(FILTER M1_DECODEPOSITIONAL_API_SRC_FILES EXCLUDE REGEX ".*Mach1AudioTimeline.*") # Exclude the Mach1AudioTimeline API

add_library(M1DecodePositional STATIC ${M1_DECODEPOSITIONAL_API_SRC_FILES} $<TARGET_OBJECTS:M1Decode>)
set_target_properties(M1DecodePositional
PROPERTIES
    VERSION ${DECODEPOSITIONAL_VERSION}
    SOVERSION ${DECODEPOSITIONAL_VERSION}
)
target_include_directories(M1DecodePositional PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/api_common/include" "${CMAKE_CURRENT_SOURCE_DIR}/api_decodepositional/include" ${M1DECODE_API_INCLUDE_PATHS} "${CMAKE_CURRENT_SOURCE_DIR}/deps" "${CMAKE_CURRENT_SOURCE_DIR}/deps/glm" "${CMAKE_CURRENT_SOURCE_DIR}/deps/pugixml/src" "${CMAKE_CURRENT_SOURCE_DIR}/deps/yaml")

#----------------
# Mach1 Transcode API
# Note: (Optional) Mach1Transcode API requires Mach1Encode API

file(GLOB M1_TRANSCODE_API_SRC_FILES ${M1_COMMON_SRC_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/matrices/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/matrices/*.tpp")
list(FILTER M1_TRANSCODE_API_SRC_FILES EXCLUDE REGEX ".*Emscripten.*") # Exclude the Emscripten files

if(M1TRANSCODE_INLINE_ENCODE)
    message(STATUS "TRANSCODE API: Supports inline encoder")
    add_library(M1Transcode STATIC ${M1_TRANSCODE_API_SRC_FILES} $<TARGET_OBJECTS:M1Encode>)
else()
    message(STATUS "TRANSCODE API: Does not support inline encoder")
    add_library(M1Transcode STATIC ${M1_TRANSCODE_API_SRC_FILES})
    target_include_directories(M1Transcode PUBLIC ${M1ENCODE_API_INCLUDE_PATHS})
endif()
set_target_properties(M1Transcode
PROPERTIES
    VERSION ${TRANSCODE_VERSION}
    SOVERSION ${TRANSCODE_VERSION}
)
target_include_directories(M1Transcode PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/api_common/include" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/include" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/matrices" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/src" "${CMAKE_CURRENT_SOURCE_DIR}/deps" "${CMAKE_CURRENT_SOURCE_DIR}/deps/yaml" "${CMAKE_CURRENT_SOURCE_DIR}/deps/pugixml/src")

#----------------
# Combined Mach1Spatial lib

# Create a combined library target
add_library(${PROJECT_NAME} STATIC
    $<TARGET_OBJECTS:M1Decode>
    $<TARGET_OBJECTS:M1Encode>
    $<TARGET_OBJECTS:M1DecodePositional>
    $<TARGET_OBJECTS:M1Transcode>
)
set_target_properties(${PROJECT_NAME}
PROPERTIES
    VERSION ${Mach1SpatialLibraries_VERSION}
    SOVERSION ${Mach1SpatialLibraries_VERSION}
)

#----------------
# Compiler flags
#----------------

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: default is Release")
endif()

message(STATUS "CMAKE_VERSION=${CMAKE_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(WIN32 OR MSVC OR MINGW)
    add_compile_definitions(M1_STATIC)
    message(STATUS "Adding windows OS flags")
endif()

#----------------
# INSTALL
#----------------

if(BUILD_MACOS_BUNDLE) # bundles for typically for Unity
    if(APPLE)
        message(STATUS "SETUP: Building .bundle style libs.")

        # Set the destination as ios for the libs
        set(LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/maccOS")
        set_target_properties(M1Decode PROPERTIES BUNDLE TRUE)
        set_target_properties(M1Decode PROPERTIES PREFIX "lib")
        set_target_properties(M1Decode PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
        set_target_properties(M1Decode PROPERTIES SUFFIX ".bundle")

        set_target_properties(M1DecodePositional PROPERTIES BUNDLE TRUE)
        set_target_properties(M1DecodePositional PROPERTIES PREFIX "lib")
        set_target_properties(M1DecodePositional PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
        set_target_properties(M1DecodePositional PROPERTIES SUFFIX ".bundle")

        message(STATUS "INSTALL: Installing bundle libs to ${LIB_DEST_DIR}.")
        install (TARGETS M1Decode M1DecodePositional
            LIBRARY DESTINATION "${LIB_DEST_DIR}"
            ARCHIVE DESTINATION "${LIB_DEST_DIR}"
        )
    endif()
endif()

if(BUILD_COCOAPODS_LIBS)
    message(STATUS "SETUP: Building for Cocoapods")

    if(IOS OR ARCHS STREQUAL OS OR ARCHS STREQUAL OS64 OR ARCHS STREQUAL SIMULATOR OR ARCHS STREQUAL SIMULATOR64 OR ARCHS STREQUAL SIMULATORARM64 OR ARCHS STREQUAL MAC_CATALYST OR ARCHS STREQUAL MAC_CATALYST_ARM64 OR ARCHS STREQUAL MAC_CATALYST_UNIVERSAL)
        # Set the destination as ios for the libs
        set(IOS_LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios")
    elseif(APPLE OR ARCHS STREQUAL MAC OR ARCHS STREQUAL MAC_ARM64 OR ARCHS STREQUAL MAC_UNIVERSAL)
        # Set the destination as osx for the libs
        set(IOS_LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/osx")
    elseif(ARCHS STREQUAL TVOS OR ARCHS STREQUAL SIMULATOR_TVOS OR ARCHS STREQUAL SIMULATORARM64_TVOS)
    elseif(ARCHS STREQUAL WATCHOS OR ARCHS STREQUAL SIMULATOR_WATCHOS OR ARCHS STREQUAL SIMULATORARM64_WATCHOS)
    elseif()
        message(WARNING "SETUP: Did not find the correct osx target.")
    endif()

    # Rename the output .a files to match the old naming (TODO: decide on updating the naming univerally)
    set_target_properties(M1Decode PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
    set_target_properties(M1DecodePositional PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
    set_target_properties(M1Encode PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
    set_target_properties(M1Transcode PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")

    # Create the destination directory if it doesn't exist
    file(MAKE_DIRECTORY "${IOS_LIB_DEST_DIR}")
    message(STATUS "INSTALL dest dir: ${IOS_LIB_DEST_DIR}")

    if(EXISTS "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI")
        message(STATUS "INSTALL: Installing includes and classes into cocoapods example.")

        # Define the source directory and the list of example directories
        set(SOURCE_DIRS
            "${CMAKE_SOURCE_DIR}/libmach1spatial/api_decode/include"
            "${CMAKE_SOURCE_DIR}/libmach1spatial/api_decodepositional/include"
            "${CMAKE_SOURCE_DIR}/libmach1spatial/api_encode/include"
            "${CMAKE_SOURCE_DIR}/libmach1spatial/api_transcode/include"
        )

        file(COPY "${CMAKE_SOURCE_DIR}/libmach1spatial/api_common/swift/"
            DESTINATION "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes"
            FILES_MATCHING
            PATTERN "*.h"
            PATTERN "*.swift"
        )

        # Loop through each api directory and copy the swift related source files
        foreach(SOURCE_DIR ${SOURCE_DIRS})
            file(COPY "${SOURCE_DIR}/"
                DESTINATION "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes"
                FILES_MATCHING
                PATTERN "*CAPI.h"
                PATTERN "*.swift"
                PATTERN "*js*" EXCLUDE
            )
        endforeach()

        message(STATUS "INSTALL: Install target set to install libs into cocoapods example.")
        install (TARGETS M1Decode M1DecodePositional M1Encode M1Transcode
            LIBRARY DESTINATION "${IOS_LIB_DEST_DIR}"
            ARCHIVE DESTINATION "${IOS_LIB_DEST_DIR}"
        )
    endif()
endif()

if(BUILD_JITPACK_LIBS)
    message(STATUS "SETUP: Building for JitPack")

    if (CMAKE_SYSTEM_NAME STREQUAL "Android")
        message(STATUS "SETUP: Additional Android flags set")

        # ANDROID macro is not defined by CMake 3.7+, however it is used by
        # some packages like OpenCV
        # (https://gitlab.kitware.com/cmake/cmake/merge_requests/62)
        add_definitions("-DANDROID")

        message(STATUS "[Android] CMAKE_ANDROID_NDK='${ANDROID_ABI}'")
        message(STATUS "[Android] CMAKE_ANDROID_ARCH_ABI='${CMAKE_ANDROID_ARCH_ABI}'")
        message(STATUS "[Android] CMAKE_ANDROID_ARCH='${CMAKE_ANDROID_ARCH}'")
    endif()

    set(ANDROID_LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/${ANDROID_ABI}")
    message(STATUS "Android Setup: lib install dir=${ANDROID_LIB_DEST_DIR}")

    if(EXISTS "${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake")
        message(STATUS "SETUP: Found Android NDK and toolchain at ${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake")
    else()
        message(FATAL_ERROR "Unable to locate the Android NDK and Android toolchain. Please set `CMAKE_ANDROID_NDK`.")
    endif()

    # Rename the output .a files to match the old naming (TODO: decide on updating the naming univerally)
    set_target_properties(M1Decode PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
    set_target_properties(M1DecodePositional PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
    set_target_properties(M1Encode PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
    set_target_properties(M1Transcode PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")

    # Create the destination directory if it doesn't exist
    file(MAKE_DIRECTORY "${ANDROID_LIB_DEST_DIR}")
    message(STATUS "INSTALL dest dir: ${ANDROID_LIB_DEST_DIR}")

    if(EXISTS "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs")
        message(STATUS "INSTALL: Installing includes and libs into Jitpack example.")

        # Copy jni swig genreated java wrappers
        file(COPY "${CMAKE_SOURCE_DIR}/libmach1spatial/swig/jni/java"
            DESTINATION "${CMAKE_BINARY_DIR}/examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main"
        )

        # Copy jni swig genreated cpp java wrappers
        file(COPY "${CMAKE_SOURCE_DIR}/libmach1spatial/swig"
            DESTINATION "${CMAKE_BINARY_DIR}/examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1"
            FILES_MATCHING
            PATTERN "*.cxx"
        )

        # Copy CAPI includes
        set(SOURCE_DIRS
            "${CMAKE_SOURCE_DIR}/libmach1spatial/api_decode/include"
            "${CMAKE_SOURCE_DIR}/libmach1spatial/api_decodepositional/include"
            "${CMAKE_SOURCE_DIR}/libmach1spatial/api_encode/include"
            "${CMAKE_SOURCE_DIR}/libmach1spatial/api_transcode/include"
        )
        foreach(SOURCE_DIR ${SOURCE_DIRS})
            file(COPY "${SOURCE_DIR}/"
                DESTINATION "${CMAKE_BINARY_DIR}/examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1"
                FILES_MATCHING
                PATTERN "*CAPI.h"
                PATTERN "*js*" EXCLUDE
            )
        endforeach()

        message(STATUS "INSTALL: Install target set to install libs into cocoapods example.")
        install (TARGETS M1Decode M1DecodePositional M1Encode M1Transcode
            LIBRARY DESTINATION "${ANDROID_LIB_DEST_DIR}"
            ARCHIVE DESTINATION "${ANDROID_LIB_DEST_DIR}"
        )
    endif()
endif()
