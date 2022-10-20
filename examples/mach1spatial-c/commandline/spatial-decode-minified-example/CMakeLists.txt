cmake_minimum_required(VERSION 3.1.0)
project(spatial-decodeminified-example)
set(MACH1SPATIAL_LIBS_PATH "../../../../mach1spatial-libs")

if(APPLE)
    set(MACH1SPATIAL_LIBS_UNIX_PATH ${MACH1SPATIAL_LIBS_PATH}/xcode)
else()
    set(MACH1SPATIAL_LIBS_UNIX_PATH ${MACH1SPATIAL_LIBS_PATH}/linux)
endif()

# collect and add source files
set(SOURCES 
    src/main.cpp
    ${MACH1SPATIAL_LIBS_PATH}/xcode/include/Mach1Decode.cpp
    )

# create the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# link libraries
find_library(MACH1DECODE_LIBRARY 
             NAMES Mach1Decode-minifiedCAPI
             PATHS ${MACH1SPATIAL_LIBS_UNIX_PATH}/lib ${MACH1SPATIAL_LIBS_PATH}/windows-x86 ${MACH1SPATIAL_LIBS_PATH}/windows-x86_64
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)

# include headers
set(MACH1SPATIAL_INCLUDES ${MACH1SPATIAL_LIBS_PATH}/linux/include ${MACH1SPATIAL_LIBS_PATH}/linux/include/M1DSP ${MACH1SPATIAL_LIBS_PATH}/xcode/include ${MACH1SPATIAL_LIBS_PATH}/xcode/include/M1DSP ${MACH1SPATIAL_LIBS_PATH}/windows-x86/include ${MACH1SPATIAL_LIBS_PATH}/windows-x86/include/M1DSP)
include_directories(${MACH1SPATIAL_INCLUDES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1DECODE_LIBRARY})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION build)