
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

option(BUILD_M1DECODE "Build M1Decode target" ON)
option(BUILD_M1ENCODE "Build M1Encode target" ON)
option(BUILD_M1DECODEPOSITIONAL "Build M1DecodePositional target (requires M1Decode)" ON)
option(BUILD_M1TRANSCODE "Build M1Transcode target (requires M1Encode)" ON)
option(BUILD_MACH1SPATIAL_COMBINED_LIB "Build Mach1Spatial combined target" OFF)

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
# Determine Library Type Based on Platform
#----------------

# Default to not building shared libs
set(BUILD_SHARED_LIBS_FOR_UNITY FALSE)

# Determine Library Type Based on Platform and settings
if(BUILD_UNITY_LIBS)
    if(APPLE)
        message(STATUS "SETUP: Building macOS or iOS or tvOS or visionOS libs.")
        if(IOS)
            set(BUILD_SHARED_LIBS_FOR_UNITY FALSE)
            set(LIB_TYPE STATIC)
        elseif(TVOS)
            set(BUILD_SHARED_LIBS_FOR_UNITY FALSE)
            set(LIB_TYPE STATIC)
        elseif(VISIONOS)
            set(BUILD_SHARED_LIBS_FOR_UNITY FALSE)
            set(LIB_TYPE STATIC)
        else()
            message(STATUS "SETUP: Building .bundle style libs for macOS.")
            set(BUILD_SHARED_LIBS_FOR_UNITY TRUE)
            set(LIB_TYPE MODULE)  # Set the library type to MODULE for macOS bundles
        endif()
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
        message(STATUS "SETUP: Building shared libs for Android.")
        set(BUILD_SHARED_LIBS_FOR_UNITY TRUE)
        set(LIB_TYPE SHARED)
    elseif(WIN32 OR MSVC OR MINGW)
        message(STATUS "SETUP: Building shared libs for Windows.")
        set(BUILD_SHARED_LIBS_FOR_UNITY TRUE)
        set(LIB_TYPE SHARED)
    else()
        set(BUILD_SHARED_LIBS_FOR_UNITY FALSE)
        set(LIB_TYPE STATIC)
    endif()
else() 
    set(BUILD_SHARED_LIBS_FOR_UNITY FALSE)
    set(LIB_TYPE STATIC)   
endif()

#----------------
# APIs
#----------------

project(Mach1Spatial VERSION ${Mach1SpatialLibraries_VERSION})

#----------------
# Common Files

set(M1_DSP_UTILITIES_SRC_FILES
    deps/M1DSP/M1DSPDynamics.h
    deps/M1DSP/M1DSPDynamics.cpp
    deps/M1DSP/M1DSPFilters.h
    deps/M1DSP/M1DSPFilters.cpp
    deps/M1DSP/M1DSPUtilities.h
    deps/M1DSP/M1DSPUtilities.cpp
)

# List of shared source files
set(M1_SHARED_SRC_FILES    
    api_common/include/Mach1Point3D.h
    api_common/include/Mach1Point4D.h
)

# M1 object library for the shared code
# TODO: Move m1-mathematics here
add_library(M1Shared OBJECT ${M1_SHARED_SRC_FILES} ${M1_DSP_UTILITIES_SRC_FILES})

#----------------
# Mach1 Decode API

file(GLOB M1_DECODE_API_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/src/*.cpp")
list(FILTER M1_DECODE_API_SRC_FILES EXCLUDE REGEX ".*Emscripten.*") # Exclude the Emscripten files

if(BUILD_M1DECODE)
    add_library(M1Decode ${LIB_TYPE} 
        ${M1_DECODE_API_SRC_FILES}
        $<TARGET_OBJECTS:M1Shared>
    )
    if(NOT (APPLE AND BUILD_SHARED_LIBS_FOR_UNITY))
        set_target_properties(M1Decode
        PROPERTIES
            VERSION ${DECODE_VERSION}
            SOVERSION ${DECODE_VERSION}
        )
    endif()
    target_include_directories(M1Decode PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/api_common/include" "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/include")
endif()

# Set include paths for other APIs
set(M1DECODE_API_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/src" "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/include")

#----------------
# Mach1 Encode API
# Note: (Optional) Some functions in Mach1Encode API may optionally require Mach1Decode API

file(GLOB M1_ENCODE_API_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/src/*.cpp")
list(FILTER M1_ENCODE_API_SRC_FILES EXCLUDE REGEX ".*Emscripten.*") # Exclude the Emscripten files

if(BUILD_M1ENCODE)
    add_library(M1Encode ${LIB_TYPE} 
        ${M1_ENCODE_API_SRC_FILES}
        $<TARGET_OBJECTS:M1Shared>
    )
    if(NOT (APPLE AND BUILD_SHARED_LIBS_FOR_UNITY))
        set_target_properties(M1Encode
        PROPERTIES
            VERSION ${ENCODE_VERSION}
            SOVERSION ${ENCODE_VERSION}
        )
    endif()
    target_include_directories(M1Encode PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/api_common/include" "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/include")

    # Supports inline decode functions in M1Encode API
    # (Requires linking decode api)
    if(M1ENCODE_INLINE_DECODE)
        message(STATUS "ENCODE API: Supports inline decoder")
        # Only link M1Decode if it is not a MODULE library
        if(NOT LIB_TYPE STREQUAL "MODULE")
            target_link_libraries(M1Encode PUBLIC M1Decode)
        else()
            message(STATUS "Skipping linking M1Decode as it's a MODULE library.")
        endif()
        target_include_directories(M1Encode PUBLIC ${M1DECODE_API_INCLUDE_PATHS})
    else()
        message(STATUS "ENCODE API: Does not support inline decoding")
    endif()
endif()

# Set include paths for other APIs
set(M1ENCODE_API_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/src" "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/include")

#----------------
# Mach1 Decode Positional API
# Note: Mach1DecodePositional API requires Mach1Decode API

file(GLOB M1_DECODEPOSITIONAL_API_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/api_decodepositional/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_decodepositional/src/*.cpp")
list(FILTER M1_DECODEPOSITIONAL_API_SRC_FILES EXCLUDE REGEX ".*Emscripten.*") # Exclude the Emscripten files

if(BUILD_M1DECODEPOSITIONAL)
    if(NOT BUILD_M1DECODE)
        message(FATAL_ERROR "M1DecodePositional requires M1Decode, but BUILD_M1DECODE is OFF")
    endif()

    # If building a MODULE library (macOS .bundle), include M1Decode sources directly
    if(LIB_TYPE STREQUAL "MODULE")
        message(STATUS "Including M1Decode sources directly into M1DecodePositional for MODULE library")
        add_library(M1DecodePositional ${LIB_TYPE}
            ${M1_DECODEPOSITIONAL_API_SRC_FILES}
            ${M1_DECODE_API_SRC_FILES}  # Include M1Decode sources
            $<TARGET_OBJECTS:M1Shared>
        )
    else()
        # For other library types, link to M1Decode
        add_library(M1DecodePositional ${LIB_TYPE}
            ${M1_DECODEPOSITIONAL_API_SRC_FILES}
            $<TARGET_OBJECTS:M1Shared>
        )
        target_link_libraries(M1DecodePositional PUBLIC M1Decode)
    endif()

    # Set target properties and include directories
    if(NOT (APPLE AND BUILD_SHARED_LIBS_FOR_UNITY))
        set_target_properties(M1DecodePositional
        PROPERTIES
            VERSION ${DECODEPOSITIONAL_VERSION}
            SOVERSION ${DECODEPOSITIONAL_VERSION}
        )
    endif()

    target_include_directories(M1DecodePositional PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/api_common/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/api_decodepositional/include"
        ${M1DECODE_API_INCLUDE_PATHS}
        "${CMAKE_CURRENT_SOURCE_DIR}/deps"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/glm"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/pugixml/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/yaml"
    )
endif()

#----------------
# Mach1 Transcode API
# Note: (Optional) Mach1Transcode API requires Mach1Encode API

file(GLOB M1_TRANSCODE_API_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/matrices/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/matrices/*.tpp")
list(FILTER M1_TRANSCODE_API_SRC_FILES EXCLUDE REGEX ".*Emscripten.*") # Exclude the Emscripten files

if(BUILD_M1TRANSCODE)
    if(NOT BUILD_M1ENCODE AND M1TRANSCODE_INLINE_ENCODE)
        message(FATAL_ERROR "M1Transcode requires M1Encode, but BUILD_M1ENCODE is OFF")
    endif()

    add_library(M1Transcode ${LIB_TYPE} 
        ${M1_TRANSCODE_API_SRC_FILES}
        $<TARGET_OBJECTS:M1Shared>
    )

    if(M1TRANSCODE_INLINE_ENCODE)
        message(STATUS "TRANSCODE API: Supports inline encoder")
        # Only link M1Encode if it is not a MODULE library
        if(NOT LIB_TYPE STREQUAL "MODULE")
            target_link_libraries(M1Transcode PUBLIC M1Encode)
        else()
            message(STATUS "Skipping linking M1Encode as it's a MODULE library.")
        endif()
        target_include_directories(M1Transcode PUBLIC ${M1ENCODE_API_INCLUDE_PATHS})
    else()
        message(STATUS "TRANSCODE API: Does not support inline encoder")
    endif()

    if(NOT (APPLE AND BUILD_SHARED_LIBS_FOR_UNITY))
        set_target_properties(M1Transcode
        PROPERTIES
            VERSION ${TRANSCODE_VERSION}
            SOVERSION ${TRANSCODE_VERSION}
        )
    endif()

    target_include_directories(M1Transcode PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/api_common/include" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/include" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/matrices" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/src" "${CMAKE_CURRENT_SOURCE_DIR}/deps" "${CMAKE_CURRENT_SOURCE_DIR}/deps/yaml" "${CMAKE_CURRENT_SOURCE_DIR}/deps/pugixml/src")
endif()

#----------------
# Combined Mach1Spatial lib

if(BUILD_MACH1SPATIAL_COMBINED_LIB)
    # Create a combined library target
    set(MACH1SPATIAL_OBJECTS)
    list(APPEND MACH1SPATIAL_OBJECTS $<TARGET_OBJECTS:M1Shared>)
    if(BUILD_M1DECODE)
        list(APPEND MACH1SPATIAL_OBJECTS $<TARGET_OBJECTS:M1Decode>)
    endif()
    if(BUILD_M1ENCODE)
        list(APPEND MACH1SPATIAL_OBJECTS $<TARGET_OBJECTS:M1Encode>)
    endif()
    if(BUILD_M1DECODEPOSITIONAL)
        list(APPEND MACH1SPATIAL_OBJECTS $<TARGET_OBJECTS:M1DecodePositional>)
    endif()
    if(BUILD_M1TRANSCODE)
        list(APPEND MACH1SPATIAL_OBJECTS $<TARGET_OBJECTS:M1Transcode>)
    endif()

    add_library(${PROJECT_NAME} STATIC ${MACH1SPATIAL_OBJECTS})

    set_target_properties(${PROJECT_NAME}
    PROPERTIES
        VERSION ${Mach1SpatialLibraries_VERSION}
        SOVERSION ${Mach1SpatialLibraries_VERSION}
    )
endif()

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

if(BUILD_UNITY_LIBS) # Bundles typically for Unity
    # Rename the output files to match the old naming (TODO: decide on updating the naming universally)
    set_target_properties(M1Decode PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
    set_target_properties(M1DecodePositional PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
    set_target_properties(M1Encode PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
    set_target_properties(M1Transcode PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")

    if(APPLE)
        if(IOS)
            message(STATUS "SETUP: Building for iOS!")
            set(LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/iOS")
        elseif(TVOS)
            # TODO: Figure out why this doesnt catch
            message(STATUS "SETUP: Building for tvOS!")
            set(LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/tvOS")
        elseif(VISIONOS)
            if (PLATFORM STREQUAL "SIMULATOR_VISIONOS")
                message(STATUS "SETUP: Building for visionOS Simulator!")
                set(LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/visionOS/Simulator")
            else()
                message(STATUS "SETUP: Building for visionOS!")
                set(LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/visionOS/Device")
            endif()
        else()
            message(STATUS "SETUP: Building for macOS!")
            set(LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS")
            # Set additional properties for macOS .bundle files
            set_target_properties(M1Decode PROPERTIES
                BUNDLE TRUE
                PREFIX "lib"
                OUTPUT_NAME "Mach1DecodeCAPI"
                SUFFIX ".bundle"
            )
            set_target_properties(M1DecodePositional PROPERTIES
                BUNDLE TRUE
                PREFIX "lib"
                OUTPUT_NAME "Mach1DecodePositionalCAPI"
                SUFFIX ".bundle"
            )
            set_target_properties(M1Encode PROPERTIES
                BUNDLE TRUE
                PREFIX "lib"
                OUTPUT_NAME "Mach1EncodeCAPI"
                SUFFIX ".bundle"
            )
            set_target_properties(M1Transcode PROPERTIES
                BUNDLE TRUE
                PREFIX "lib"
                OUTPUT_NAME "Mach1TranscodeCAPI"
                SUFFIX ".bundle"
            )
        endif()
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
        # Android libs
        add_definitions("-DANDROID")
        message(STATUS "SETUP: Additional Android flags set")
        message(STATUS "[Android] CMAKE_ANDROID_NDK='${ANDROID_ABI}'")
        message(STATUS "[Android] CMAKE_ANDROID_ARCH_ABI='${CMAKE_ANDROID_ARCH_ABI}'")
        message(STATUS "[Android] CMAKE_ANDROID_ARCH='${CMAKE_ANDROID_ARCH}'")
        set(LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/Android/${ANDROID_ABI}")
        if(EXISTS "${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake")
            message(STATUS "SETUP: Found Android NDK and toolchain at ${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake")
        else()
            message(FATAL_ERROR "Unable to locate the Android NDK and Android toolchain. Please set `CMAKE_ANDROID_NDK`.")
        endif()
    elseif(WIN32 OR MSVC OR MINGW)
        set(vcpkg "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        if(NOT CMAKE_TOOLCHAIN_FILE AND EXISTS "${vcpkg}")
          set(CMAKE_TOOLCHAIN_FILE "${vcpkg}"
              CACHE FILEPATH "CMake toolchain file")
          message(STATUS "vcpkg toolchain found: ${CMAKE_TOOLCHAIN_FILE}")
        endif()

        # Windows libs
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            # 64-bit build
            set(LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/Windows/x86_64")
        else()
            # 32-bit build
            set(LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/Windows/x86")
        endif()
    endif()

    # Install the libraries
    if(EXISTS "${LIB_DEST_DIR}")
            message(STATUS "INSTALL: Installing libraries to ${LIB_DEST_DIR}.")
            install(TARGETS M1Decode M1DecodePositional M1Encode M1Transcode
                BUNDLE DESTINATION "${LIB_DEST_DIR}"
                LIBRARY DESTINATION "${LIB_DEST_DIR}"
                ARCHIVE DESTINATION "${LIB_DEST_DIR}"
            )
    endif()
endif()

if(BUILD_COCOAPODS_LIBS)
    message(STATUS "SETUP: Building for Cocoapods")

    if(APPLE)
        if(VISIONOS)
            if (PLATFORM STREQUAL "SIMULATOR_VISIONOS")
                message(STATUS "SETUP: Building for visionOS Simulator!")
                set(IOS_LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/device")
            else()
                message(STATUS "SETUP: Building for visionOS!")
                set(IOS_LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/simulator")
            endif()
        elseif(TVOS OR PLATFORM STREQUAL "TVOS" OR PLATFORM STREQUAL "TVOSCOMBINED")
            message(STATUS "SETUP: Building for tvOS!")
            set(IOS_LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/tvos")
        elseif(WATCHOS OR PLATFORM STREQUAL "WATCHOS" OR PLATFORM STREQUAL "WATCHOSCOMBINED")
            message(STATUS "SETUP: Building for watchOS!")
            set(IOS_LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/watchos")
        elseif(IOS OR PLATFORM STREQUAL "OS64" OR PLATFORM STREQUAL "OS64COMBINED")
            message(STATUS "SETUP: Building for iOS!")
            set(IOS_LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios")
        elseif(PLATFORM STREQUAL "MAC_UNIVERSAL")
            # Set the destination as osx for the libs after falling through the above choices first
            set(IOS_LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/osx")
        else()
            message(WARNING "SETUP: Could not find the appropriate SDK platform")
            message(WARNING "SETUP: Not using the ios-cmake toolchain is not recommended")
        endif()
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

        message(STATUS "INSTALL: Install target set to install libs into jitpack example.")
        install (TARGETS M1Decode M1DecodePositional M1Encode M1Transcode
            LIBRARY DESTINATION "${ANDROID_LIB_DEST_DIR}"
            ARCHIVE DESTINATION "${ANDROID_LIB_DEST_DIR}"
        )
    endif()
endif()
