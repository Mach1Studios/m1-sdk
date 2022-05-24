cmake_minimum_required(VERSION 3.19.3)
project(spatial-transcode-example)
set(CMAKE_CXX_STANDARD 11)

set(MACH1SPATIAL_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../")

if(WIN32 OR MSVC)
    if (NOT "$ENV{VCPKG_PATH}" STREQUAL "")
        set(VCPKG_PATH "$ENV{VCPKG_PATH}" CACHE INTERNAL "Copied from environment variable")
        message("VCPKG_PATH is set: ${VCPKG_PATH}")
        include(${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            # 64 bits
            set(MACH1SPATIAL_LIBS_PATH ${MACH1SPATIAL_SDK_PATH}/mach1spatial-libs/vs-15-2017-x86_64)
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
            # 32 bits
            set(MACH1SPATIAL_LIBS_PATH ${MACH1SPATIAL_SDK_PATH}/mach1spatial-libs/vs-15-2017-x86)
        endif()
    else()
        message(FATAL_ERROR "unexpected: the local VCPKG_PATH environment variable is not set or is empty!")
    endif()
elseif(APPLE)
    set(MACH1SPATIAL_LIBS_PATH ${MACH1SPATIAL_SDK_PATH}/mach1spatial-libs/xcode)
elseif(UNIX AND NOT APPLE)
    set(MACH1SPATIAL_LIBS_PATH ${MACH1SPATIAL_SDK_PATH}/mach1spatial-libs/linux)
else()
    set(MACH1SPATIAL_LIBS_PATH ${MACH1SPATIAL_SDK_PATH}/mach1spatial-libs/linux)
endif()
message("MACH1SPATIAL_LIBS_PATH is set: ${MACH1SPATIAL_LIBS_PATH}")

# collect and add source files
set(SOURCES
	src/main.cpp
	${MACH1SPATIAL_LIBS_PATH}/include/Mach1Transcode.cpp
	)

# create the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# link Mach1Spatial libraries
if(WIN32 OR MSVC)
  # Definition added for windows to check for static lib usage
  add_definitions(-DM1_STATIC)
  # Change `PATH` to reflect `/MT` or `/MD` as needed!
  # Mach1Transcode
  set(MACH1TRANSCODE_LIBRARY_DEBUG ${MACH1SPATIAL_LIBS_PATH}/lib/Static/MD/Debug/Mach1TranscodeCAPId.lib)
  set(MACH1TRANSCODE_LIBRARY_RELEASE ${MACH1SPATIAL_LIBS_PATH}/lib/Static/MD/Release/Mach1TranscodeCAPI.lib)
	set(MACH1TRANSCODE_LIBRARY debug ${MACH1TRANSCODE_LIBRARY_DEBUG} optimized ${MACH1TRANSCODE_LIBRARY_RELEASE})
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1TRANSCODE_LIBRARY})
else()
	find_library(MACH1TRANSCODE_LIBRARY
		           NAMES Mach1TranscodeCAPI libMach1TranscodeCAPI libMach1TranscodeCAPI.a libMach1TranscodeCAPI.so
		           PATH ${MACH1SPATIAL_LIBS_PATH}/lib
	)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1TRANSCODE_LIBRARY})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)

# include headers
set(MACH1SPATIAL_INCLUDES ${MACH1SPATIAL_LIBS_PATH}/include)
include_directories(${MACH1SPATIAL_INCLUDES})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION build)
