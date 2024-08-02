#----------------
# Setup
#----------------

cmake_minimum_required(VERSION 3.1.0)
project(Mach1Spatial)

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

if(DEFINED BUILD_MACOS_BUNDLE)
    set(BUILD_MACOS_BUNDLE 1)
    message(STATUS "SETUP: Building .bundle libs")
endif()

if(DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS 1)
    message(STATUS "SETUP: Building shared libs")
endif()

if(DEFINED BUILD_WIN_MT)
    set(BUILD_WIN_MT 1)
    message(STATUS "SETUP: Building /MT static libs for windows target")
endif()

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

# Prepend our CMake modules directory
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
message(STATUS "CMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'")

# include paths
target_include_directories(${PROJECT_NAME} 
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/cpp>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
include(GNUInstallDirs)

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
# Sources
#----------------

# include headers
set(MACH1_HEADERS 
    Mach1Point3D.h
    Mach1Point4D.h
    )
include(deps)

# collect and add source files
set(Mach1EncodeAPI_SOURCE "Mach1EncodeCAPI.cpp" "Mach1EncodeCore.cpp")
set(Mach1DecodeAPI_SOURCE "Mach1DecodeCAPI.cpp" "Mach1DecodeCore.cpp")
set(Mach1DecodePositionalAPI_SOURCE "Mach1DecodePositionalCAPI.cpp" "Mach1DecodeCore.cpp" "Mach1DecodePositionalCore.cpp")
set(Mach1TranscodeAPI_SOURCE "Mach1TranscodeCAPI.cpp" "Mach1TranscodeCore.cpp" "Mach1EncodeCore.cpp" "Mach1GenerateCoeffs.cpp" "Mach1AudioTimelineCore.cpp" "Mach1AudioTimelineCAPI.cpp")

# create the library
if(NOT SKIP_DECODE_API)
    list(APPEND MACH1_HEADERS Mach1DecodeCore.h Mach1DecodeCAPI.h)
    add_library(Mach1DecodeAPI STATIC ${MACH1_HEADERS} ${Mach1DecodeAPI_SOURCE})
endif()
if(NOT SKIP_DECODEPOSITIONAL_API)
    list(APPEND MACH1_HEADERS Mach1DecodeCAPI.h Mach1DecodePositionalCore.h Mach1DecodePositionalCAPI.h)
    add_library(Mach1DecodePositionalAPI STATIC ${MACH1_HEADERS} ${Mach1DecodePositionalAPI_SOURCE})
endif()
if(NOT SKIP_ENCODE_API)
    list(APPEND MACH1_HEADERS Mach1DecodeCAPI.h Mach1EncodeCore.h Mach1EncodeCAPI.h)
    add_library(Mach1EncodeAPI STATIC ${MACH1_HEADERS} ${Mach1EncodeAPI_SOURCE})
endif()
if(NOT SKIP_TRANSCODE_API)
    include(transcode)
    list(APPEND MACH1_HEADERS Mach1DecodeCAPI.h Mach1EncodeCore.h Mach1GenerateCoeffs.h Mach1AudioTimelineCore.h Mach1AudioTimelineCAPI.h Mach1TranscodeCore.h Mach1TranscodeCAPI.h)
    add_library(Mach1TranscodeAPI STATIC ${MACH1_HEADERS} ${Mach1TranscodeAPI_SOURCE})
endif()

#----------------
# Compiler flags
#----------------

set(COMMON_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS ${COMMON_FLAGS} "-fvisibility=hidden")

if(APPLE)
    message(STATUS "SETUP: Additional MacOS flags set")
    set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    message(STATUS "SETUP: Additional Android flags set")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -Wl,-Bstatic")

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

if(MSVC OR WIN32)
    if(BUILD_WIN_MT)
        add_compile_options(
            $<$<CONFIG:>:/MT>
            $<$<CONFIG:Debug>:/MTd>
            $<$<CONFIG:Release>:/MT>
        )
        if(NOT SKIP_DECODE_API)
            set_property(TARGET Mach1DecodeAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
        endif()
        if(NOT SKIP_DECODEPOSITIONAL_API)
            set_property(TARGET Mach1DecodePositionalAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
        endif()
        if(NOT SKIP_ENCODE_API)
            set_property(TARGET Mach1EncodeAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
        endif()
        if(NOT SKIP_TRANSCODE_API)
            set_property(TARGET Mach1TranscodeAPI PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
        endif()
    endif()
endif()

if(IOS)
    string(COMPARE EQUAL "$ENV{IOS_DEVELOPMENT_TEAM}" "" _is_empty)
    if(_is_empty)
        message("ERROR: Environment variable IOS_DEVELOPMENT_TEAM is empty")
    endif()
    set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "$ENV{IOS_DEVELOPMENT_TEAM}")

    string(COMPARE EQUAL "$ENV{IOS_BUNDLE_IDENTIFIER}" "" _is_empty)  
    if(_is_empty)
        message("ERROR: Environment variable IOS_BUNDLE_IDENTIFIER is empty")
    else()
        set(MACOSX_BUNDLE_GUI_IDENTIFIER $ENV{IOS_BUNDLE_IDENTIFIER})
    endif()
    if(NOT SKIP_DECODE_API)
        set_target_properties(Mach1DecodeAPI PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
        set_target_properties(Mach1DecodeAPI PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
    endif()
    if(NOT SKIP_DECODEPOSITIONAL_API)
        set_target_properties(Mach1DecodePositionalAPI PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
        set_target_properties(Mach1DecodePositionalAPI PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
    endif()
    if(NOT SKIP_ENCODE_API)
        set_target_properties(Mach1EncodeAPI PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
        set_target_properties(Mach1EncodeAPI PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
    endif()
    if(NOT SKIP_TRANSCODE_API)
        set_target_properties(Mach1TranscodeAPI PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
        set_target_properties(Mach1TranscodeAPI PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
    endif()

endif()

if(NOT SKIP_DECODE_API)
    set_target_properties(Mach1DecodeAPI PROPERTIES VERSION ${DECODE_VERSION})
    set_target_properties(Mach1DecodeAPI PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
    set_target_properties(Mach1DecodeAPI PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
endif()
if(NOT SKIP_DECODEPOSITIONAL_API)
    set_target_properties(Mach1DecodePositionalAPI PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
    set_target_properties(Mach1DecodePositionalAPI PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
    set_target_properties(Mach1DecodePositionalAPI PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
endif()
if(NOT SKIP_ENCODE_API)
    set_target_properties(Mach1EncodeAPI PROPERTIES VERSION ${ENCODE_VERSION})
    set_target_properties(Mach1EncodeAPI PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
    set_target_properties(Mach1EncodeAPI PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
endif()
if(NOT SKIP_TRANSCODE_API)
    set_target_properties(Mach1TranscodeAPI PROPERTIES VERSION ${TRANSCODE_VERSION})
    set_target_properties(Mach1TranscodeAPI PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")
    set_target_properties(Mach1TranscodeAPI PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
endif()

#----------------
# Link
#----------------

if(NOT SKIP_DECODEPOSITIONAL_API)
    target_include_directories(Mach1DecodePositionalAPI PRIVATE ${RootAdditionalIncludes} ${Mach1DecodePositionalAdditionalIncludes})
endif()
if(NOT SKIP_TRANSCODE_API)
    target_include_directories(Mach1TranscodeAPI PRIVATE ${RootAdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})
endif()

# UNITY / SHARED libs
if(BUILD_SHARED_LIBS)
    if(NOT SKIP_DECODE_API)
        add_library(Mach1DecodeCAPIshared SHARED ${MACH1_HEADERS} ${Mach1DecodeAPI_SOURCE})
        set_target_properties(Mach1DecodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
        set_target_properties(Mach1DecodeCAPIshared PROPERTIES VERSION ${DECODE_VERSION})
        install(TARGETS Mach1DecodeCAPIshared DESTINATION lib-shared)
    endif()
    if(NOT SKIP_DECODEPOSITIONAL_API)
        add_library(Mach1DecodePositionalCAPIshared SHARED ${MACH1_HEADERS} ${Mach1DecodePositionalAPI_SOURCE})
        target_include_directories(Mach1DecodePositionalCAPIshared PRIVATE ${RootAdditionalIncludes} ${Mach1DecodePositionalAdditionalIncludes})
        set_target_properties(Mach1DecodePositionalCAPIshared PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
        set_target_properties(Mach1DecodePositionalCAPIshared PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
        install(TARGETS Mach1DecodePositionalCAPIshared DESTINATION lib-shared)
    endif()
    if(NOT SKIP_ENCODE_API)
        add_library(Mach1EncodeCAPIshared SHARED ${MACH1_HEADERS} ${Mach1EncodeAPI_SOURCE})
        set_target_properties(Mach1EncodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
        set_target_properties(Mach1EncodeCAPIshared PROPERTIES VERSION ${ENCODE_VERSION})
        install(TARGETS Mach1EncodeCAPIshared DESTINATION lib-shared)
    endif()
    if(NOT SKIP_TRANSCODE_API)
        add_library(Mach1TranscodeCAPIshared SHARED ${MACH1_HEADERS} ${Mach1TranscodeAPI_SOURCE})
        target_include_directories(Mach1TranscodeCAPIshared PRIVATE ${RootAdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})
        set_target_properties(Mach1TranscodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")
        set_target_properties(Mach1TranscodeCAPIshared PROPERTIES VERSION ${TRANSCODE_VERSION})
        install(TARGETS Mach1TranscodeCAPIshared DESTINATION lib-shared)
    endif()
endif()

if(BUILD_MACOS_BUNDLE)
    # bundles for Unity
    if(NOT SKIP_DECODE_API)
        add_library(Mach1DecodeCAPIbundle MODULE ${MACH1_HEADERS} ${Mach1DecodeAPI_SOURCE})
        set_target_properties(Mach1DecodeCAPIbundle PROPERTIES BUNDLE TRUE)
        set_target_properties(Mach1DecodeCAPIbundle PROPERTIES PREFIX "lib")
        set_target_properties(Mach1DecodeCAPIbundle PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
        set_target_properties(Mach1DecodeCAPIbundle PROPERTIES SUFFIX ".bundle")
        set_target_properties(Mach1DecodeCAPIbundle PROPERTIES VERSION ${DECODE_VERSION})
        install(TARGETS Mach1DecodeCAPIbundle DESTINATION lib-bundle)
    endif()
    if(NOT SKIP_DECODEPOSITIONAL_API)
        add_library(Mach1DecodePositionalCAPIbundle MODULE ${MACH1_HEADERS} ${Mach1DecodePositionalAPI_SOURCE})
        target_include_directories(Mach1DecodePositionalCAPIbundle PRIVATE ${RootAdditionalIncludes} ${Mach1DecodePositionalAdditionalIncludes})
        set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES BUNDLE TRUE)
        set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES PREFIX "lib")
        set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
        set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES SUFFIX ".bundle")
        set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
        install(TARGETS Mach1DecodePositionalCAPIbundle DESTINATION lib-bundle)
    endif()
    if(NOT SKIP_ENCODE_API)
        add_library(Mach1EncodeCAPIbundle MODULE ${MACH1_HEADERS} ${Mach1EncodeAPI_SOURCE})
        set_target_properties(Mach1EncodeCAPIbundle PROPERTIES BUNDLE TRUE)
        set_target_properties(Mach1EncodeCAPIbundle PROPERTIES PREFIX "lib")
        set_target_properties(Mach1EncodeCAPIbundle PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
        set_target_properties(Mach1EncodeCAPIbundle PROPERTIES SUFFIX ".bundle")
        set_target_properties(Mach1EncodeCAPIbundle PROPERTIES VERSION ${ENCODE_VERSION})
        install(TARGETS Mach1EncodeCAPIbundle DESTINATION lib-bundle)
    endif()
    if(NOT SKIP_TRANSCODE_API)
        add_library(Mach1TranscodeCAPIbundle MODULE ${MACH1_HEADERS} ${Mach1TranscodeAPI_SOURCE})
        target_include_directories(Mach1TranscodeCAPIbundle PRIVATE ${RootAdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})
        set_target_properties(Mach1TranscodeCAPIbundle PROPERTIES BUNDLE TRUE)
        set_target_properties(Mach1TranscodeCAPIbundle PROPERTIES PREFIX "lib")
        set_target_properties(Mach1TranscodeCAPIbundle PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")
        set_target_properties(Mach1TranscodeCAPIbundle PROPERTIES SUFFIX ".bundle")
        set_target_properties(Mach1TranscodeCAPIbundle PROPERTIES VERSION ${TRANSCODE_VERSION})
        install(TARGETS Mach1TranscodeCAPIbundle DESTINATION lib-bundle)
    endif()
endif()

if(IOS)
    # STATIC
    if(NOT SKIP_DECODE_API)
        add_library(Mach1DecodeCAPIframework STATIC ${MACH1_HEADERS} ${Mach1DecodeAPI_SOURCE})
        set_target_properties(Mach1DecodeCAPIframework PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
        set_target_properties(Mach1DecodeCAPIframework PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
        set_target_properties(Mach1DecodeCAPIframework PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
        set_target_properties(Mach1DecodeCAPIframework PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)
        set_target_properties(Mach1DecodeCAPIframework PROPERTIES VERSION ${DECODE_VERSION})
    endif()
    if(NOT SKIP_DECODEPOSITIONAL_API)
        add_library(Mach1DecodePositionalCAPIframework STATIC ${MACH1_HEADERS} ${Mach1DecodePositionalAPI_SOURCE})
        target_include_directories(Mach1DecodePositionalCAPIframework PRIVATE ${RootAdditionalIncludes} ${Mach1DecodePositionalAdditionalIncludes})
        set_target_properties(Mach1DecodePositionalCAPIframework PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
        set_target_properties(Mach1DecodePositionalCAPIframework PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
        set_target_properties(Mach1DecodePositionalCAPIframework PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
        set_target_properties(Mach1DecodePositionalCAPIframework PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)
        set_target_properties(Mach1DecodePositionalCAPIframework PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})

    endif()
    if(NOT SKIP_ENCODE_API)
        add_library(Mach1EncodeCAPIframework STATIC ${MACH1_HEADERS} ${Mach1EncodeAPI_SOURCE})
        set_target_properties(Mach1EncodeCAPIframework PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
        set_target_properties(Mach1EncodeCAPIframework PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
        set_target_properties(Mach1EncodeCAPIframework PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
        set_target_properties(Mach1EncodeCAPIframework PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)
        set_target_properties(Mach1EncodeCAPIframework PROPERTIES VERSION ${ENCODE_VERSION})

    endif()
    if(NOT SKIP_TRANSCODE_API)
        add_library(Mach1TranscodeCAPIframework STATIC ${MACH1_HEADERS} ${Mach1TranscodeAPI_SOURCE})
        target_include_directories(Mach1TranscodeCAPIframework PRIVATE ${RootAdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})
        set_target_properties(Mach1TranscodeCAPIframework PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
        set_target_properties(Mach1TranscodeCAPIframework PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
        set_target_properties(Mach1TranscodeCAPIframework PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")
        set_target_properties(Mach1TranscodeCAPIframework PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)
        set_target_properties(Mach1TranscodeCAPIframework PROPERTIES VERSION ${TRANSCODE_VERSION})
    endif()

    option(BUILD_SHARED_LIBS "..." ON)
    if(NOT SKIP_DECODE_API)
        set_target_properties(
            Mach1DecodeCAPIframework
            PROPERTIES
            FRAMEWORK TRUE
        )
        install(TARGETS Mach1DecodeCAPIframework DESTINATION Frameworks)
    endif()
    if(NOT SKIP_DECODEPOSITIONAL_API)
        set_target_properties(
            Mach1DecodePositionalCAPIframework
            PROPERTIES
            FRAMEWORK TRUE
        )
        install(TARGETS Mach1DecodePositionalCAPIframework DESTINATION Frameworks)
    endif()
    if(NOT SKIP_ENCODE_API)
        set_target_properties(
            Mach1EncodeCAPIframework
            PROPERTIES
            FRAMEWORK TRUE
        )
        install(TARGETS Mach1EncodeCAPIframework DESTINATION Frameworks)
    endif()
    if(NOT SKIP_TRANSCODE_API)
        set_target_properties(
            Mach1TranscodeCAPIframework
            PROPERTIES
            FRAMEWORK TRUE
        )
        install(TARGETS Mach1TranscodeCAPIframework DESTINATION Frameworks)
    endif()
endif()

#----------------
# Install
#----------------

message(STATUS "CMAKE_INSTALL_PREFIX='${CMAKE_INSTALL_PREFIX}'")
if(NOT SKIP_DECODE_API)
    install(TARGETS Mach1DecodeAPI DESTINATION lib)
endif()
if(NOT SKIP_DECODEPOSITIONAL_API)
    install(TARGETS Mach1DecodePositionalAPI DESTINATION lib)
endif()
if(NOT SKIP_ENCODE_API)
    install(TARGETS Mach1EncodeAPI DESTINATION lib)
endif()
if(NOT SKIP_TRANSCODE_API)
    install(TARGETS Mach1TranscodeAPI DESTINATION lib)
endif()

#-------------------------------------------------------------------------------