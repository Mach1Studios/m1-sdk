cmake_minimum_required(VERSION 3.19.3)
project(spatial-transcode-example)
set(CMAKE_CXX_STANDARD 11)

set(MACH1SPATIAL_LIBS_PATH "../../../../mach1spatial-libs")

# collect and add source files
set(SOURCES 
	src/main.cpp
	${MACH1SPATIAL_LIBS_PATH}/xcode/include/Mach1Transcode.cpp
	)

# create the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# link libraries
find_library(MACH1TRANSCODE_LIBRARY 
             NAMES Mach1TranscodeCAPI libMach1TranscodeCAPI libMach1TranscodeCAPI.a libMach1TranscodeCAPI.so Mach1TranscodeCAPI.lib
             PATHS ${MACH1SPATIAL_LIBS_PATH}/xcode/lib ${MACH1SPATIAL_LIBS_PATH}/linux/lib ${MACH1SPATIAL_LIBS_PATH}/windows-x86 ${MACH1SPATIAL_LIBS_PATH}/windows-x86_64
)

# include headers
set(MACH1SPATIAL_INCLUDES ${MACH1SPATIAL_LIBS_PATH}/linux/include ${MACH1SPATIAL_LIBS_PATH}/linux/include/M1DSP ${MACH1SPATIAL_LIBS_PATH}/xcode/include ${MACH1SPATIAL_LIBS_PATH}/xcode/include/M1DSP ${MACH1SPATIAL_LIBS_PATH}/windows-x86/include ${MACH1SPATIAL_LIBS_PATH}/windows-x86/include/M1DSP)
include_directories(${MACH1SPATIAL_INCLUDES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1TRANSCODE_LIBRARY})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION build)