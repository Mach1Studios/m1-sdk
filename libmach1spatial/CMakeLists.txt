
#----------------
# Setup
#----------------

cmake_minimum_required(VERSION 3.13.0)

# Prepend our CMake modules directory
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
message(STATUS "CMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'")

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

file(GLOB M1_DECODE_API_SRC_FILES ${M1_COMMON_SRC_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_decode/src/*.cpp")
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

file(GLOB M1_ENCODE_API_SRC_FILES ${M1_COMMON_SRC_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_encode/src/*.cpp")
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
    message(STATUS "ENCODE API: Supports inline decoding")
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

file(GLOB M1_TRANSCODE_API_SRC_FILES ${M1_COMMON_SRC_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/api_transcode/matrices/*.h")
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

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif()

if(DEFINED CXX_VERSION STREQUAL 14)
    set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_STANDARD 14)
    message(STATUS "SETUP: Using C++14")
elseif(DEFINED CXX_VERSION STREQUAL 17)
    set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_STANDARD 17)
    message(STATUS "SETUP: Using C++17")
else()
    if (CMAKE_VERSION VERSION_LESS "3.1")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
            message(STATUS "SETUP: Using gnu++11")
        endif()
    else()
        if (CMAKE_SYSTEM_NAME STREQUAL "Android")
            message(STATUS "SETUP: Additional Android based c++11 flags set")
            set(CMAKE_CXX_FLAGS_INIT "-std=c++11")
        else()
            set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
        endif()
            set(CMAKE_CXX_STANDARD 11)
            set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ Standard (toolchain)" FORCE)
            set(CMAKE_CXX_STANDARD_REQUIRED YES CACHE BOOL "C++ Standard required" FORCE)
            message(STATUS "SETUP: Using C++11")
    endif()
endif()
message(STATUS "SETUP: Using ${CMAKE_CXX_COMPILER}")

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: default is Release")
endif()

message(STATUS "CMAKE_VERSION=${CMAKE_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(MSVC OR WIN32)
    set(CMAKE_DEBUG_POSTFIX d)
    # enable MSVC_RUNTIME_LIBRARY target property
    # see https://cmake.org/cmake/help/latest/policy/CMP0091.html
    if(POLICY CMP0091)
        cmake_policy(SET CMP0091 NEW)
    endif()
endif()

set(COMMON_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS ${COMMON_FLAGS} "-fvisibility=hidden")

if(APPLE)
    message(STATUS "SETUP: Additional MacOS flags set")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    message(STATUS "SETUP: Additional Android flags set")

    # ANDROID macro is not defined by CMake 3.7+, however it is used by
    # some packages like OpenCV
    # (https://gitlab.kitware.com/cmake/cmake/merge_requests/62)
    add_definitions("-DANDROID")

    message(STATUS "[Android] CMAKE_ANDROID_NDK='${CMAKE_ANDROID_NDK}'")
    message(STATUS "[Android] ANDROID_NDK_VERSION='${ANDROID_NDK_VERSION}'")
    message(STATUS "[Android] CMAKE_SYSTEM_VERSION='${CMAKE_SYSTEM_VERSION}'")
    message(STATUS "[Android] CMAKE_ANDROID_ARCH_ABI='${CMAKE_ANDROID_ARCH_ABI}'")
    message(STATUS "[Android] CMAKE_ANDROID_ARCH='${CMAKE_ANDROID_ARCH}'")
endif()

message(STATUS "CMAKE_CXX_COMPILER_VERSION='${CMAKE_CXX_COMPILER_VERSION}'")
message(STATUS "CMAKE_CXX_FLAGS='${CMAKE_CXX_FLAGS}'")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG='${CMAKE_CXX_FLAGS_DEBUG}'")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE='${CMAKE_CXX_FLAGS_RELEASE}'")
message(STATUS "CMAKE_CXX_COMPILE_OBJECT='${CMAKE_CXX_COMPILE_OBJECT}'")

#----------------
# Link
#----------------

if(MSVC OR WIN32)
    if(BUILD_WIN_MT)
        message(STATUS "Adding /MT compile options for win targets")
        add_compile_options(
            $<$<CONFIG:>:/MT>
            $<$<CONFIG:Debug>:/MTd>
            $<$<CONFIG:Release>:/MT>
        )
        set_property(TARGET Mach1DecodeAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
        set_property(TARGET Mach1DecodePositionalAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
        set_property(TARGET Mach1EncodeAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
        set_property(TARGET Mach1TranscodeAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif()
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
        set(LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios")
    elseif(APPLE OR ARCHS STREQUAL MAC OR ARCHS STREQUAL MAC_ARM64 OR ARCHS STREQUAL MAC_UNIVERSAL)
        # Set the destination as osx for the libs
        set(LIB_DEST_DIR "${CMAKE_SOURCE_DIR}/examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/osx")
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
    file(MAKE_DIRECTORY "${LIB_DEST_DIR}")
    message(STATUS "INSTALL dest dir: ${LIB_DEST_DIR}")
    
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
            LIBRARY DESTINATION "${LIB_DEST_DIR}"
            ARCHIVE DESTINATION "${LIB_DEST_DIR}"
        )
    endif()
endif()