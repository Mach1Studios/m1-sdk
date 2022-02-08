cmake_minimum_required(VERSION 3.19.3)
project(spatial-decode-example)
set(MACH1SPATIAL_LIBS_PATH "../../../../mach1spatial-libs")

# collect and add source files
set(SOURCES 
	src/main.cpp
	${MACH1SPATIAL_LIBS_PATH}/xcode/include/Mach1Decode.cpp
	)

# create the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# link libraries
find_library(MACH1DECODE_LIBRARY 
             NAMES Mach1DecodeCAPI libMach1DecodeCAPI libMach1DecodeCAPI.a libMach1DecodeCAPI.so libMach1DecodeCAPI.lib
             PATHS ${MACH1SPATIAL_LIBS_PATH}/xcode/lib ${MACH1SPATIAL_LIBS_PATH}/linux/lib ${MACH1SPATIAL_LIBS_PATH}/windows-x86 ${MACH1SPATIAL_LIBS_PATH}/windows-x86_64
)

# include headers
set(MACH1SPATIAL_INCLUDES ${MACH1SPATIAL_LIBS_PATH}/linux/include ${MACH1SPATIAL_LIBS_PATH}/linux/include/M1DSP ${MACH1SPATIAL_LIBS_PATH}/xcode/include ${MACH1SPATIAL_LIBS_PATH}/xcode/include/M1DSP ${MACH1SPATIAL_LIBS_PATH}/windows-x86/include ${MACH1SPATIAL_LIBS_PATH}/windows-x86/include/M1DSP)
include_directories(${MACH1SPATIAL_INCLUDES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MACH1DECODE_LIBRARY})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION build)