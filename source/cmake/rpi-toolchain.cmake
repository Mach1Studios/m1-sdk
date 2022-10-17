# Based on:
# https://gitlab.linphone.org/BC/public/linphone-cmake-builder/blob/master/toolchains/toolchain-raspberry.cmake

if("$ENV{RASPBERRY_VERSION}" STREQUAL "")
	set(RASPBERRY_VERSION 1)
else()
	if($ENV{RASPBERRY_VERSION} VERSION_GREATER 3)
		set(RASPBERRY_VERSION 3)
	else()
		set(RASPBERRY_VERSION $ENV{RASPBERRY_VERSION})
	endif()
endif()

# RPI_TOOLS should point to the local directory which contains all the libraries and includes from the target raspi.

if("$ENV{RPI_TOOLS}" STREQUAL "")
	message(FATAL_ERROR "Define the RPI_TOOLS environment variable to point to the rpi/tools dir.")
else()
	set(SYSROOT_PATH "$ENV{RPI_TOOLS}/")
endif()
set(TOOLCHAIN_PATH "arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf")
set(TOOLCHAIN_HOST "arm-linux-gnueabihf")

message(STATUS "Using sysroot path: ${SYSROOT_PATH}")

set(TOOLCHAIN_CC "${TOOLCHAIN_HOST}-gcc")
set(TOOLCHAIN_CXX "${TOOLCHAIN_HOST}-g++")
set(TOOLCHAIN_LD "${TOOLCHAIN_HOST}-ld")
set(TOOLCHAIN_AR "${TOOLCHAIN_HOST}-ar")
set(TOOLCHAIN_RANLIB "${TOOLCHAIN_HOST}-ranlib")
set(TOOLCHAIN_STRIP "${TOOLCHAIN_HOST}-strip")
set(TOOLCHAIN_NM "${TOOLCHAIN_HOST}-nm")

set(CMAKE_CROSSCOMPILING TRUE)

# Define name of the target system
set(CMAKE_SYSTEM_NAME "Linux")
if(RASPBERRY_VERSION VERSION_GREATER 1)
	set(CMAKE_SYSTEM_PROCESSOR "armv7")
else()
	set(CMAKE_SYSTEM_PROCESSOR "arm")
endif()

# Define the compiler
SET(CMAKE_C_COMPILER ${SYSROOT_PATH}${TOOLCHAIN_PATH}/bin/${TOOLCHAIN_CC})
SET(CMAKE_CXX_COMPILER ${SYSROOT_PATH}${TOOLCHAIN_PATH}/bin/${TOOLCHAIN_CXX})
# Define the sysroot path for the RaspberryPi distribution in our tools folder 
SET(CMAKE_FIND_ROOT_PATH ${SYSROOT_PATH}${TOOLCHAIN_PATH}/arm-linux-gnueabihf/sysroot)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")

if(RASPBERRY_VERSION VERSION_GREATER 2)
	set(CMAKE_C_FLAGS "-mcpu=cortex-a53 -mfpu=neon-vfpv4 -mfloat-abi=hard ${COMMON_FLAGS}" CACHE STRING "Flags for Raspberry PI 3")
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "Flags for Raspberry PI 3")
elseif(RASPBERRY_VERSION VERSION_GREATER 1)
	set(CMAKE_C_FLAGS "-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard ${COMMON_FLAGS}" CACHE STRING "Flags for Raspberry PI 2")
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "Flags for Raspberry PI 2")
else()
	set(CMAKE_C_FLAGS "-mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard ${COMMON_FLAGS}" CACHE STRING "Flags for Raspberry PI 1 B+ Zero")
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "Flags for Raspberry PI 1 B+ Zero")
endif()

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)