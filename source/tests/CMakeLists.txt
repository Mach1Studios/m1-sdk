#----------------
# Setup
#----------------

cmake_minimum_required(VERSION 3.5)
project(Mach1SpatialSDK_Tests)
set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: default is Release")
endif()

message(STATUS "CMAKE_VERSION=${CMAKE_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if (APPLE)
	#set(CMAKE_OSX_SYSROOT macosx11.0)
	#set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14)
	set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
	set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
endif()

message("CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")
message("CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message("CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message("CMAKE_CROSSCOMPILING: ${CMAKE_CROSSCOMPILING}")

# Prepend our CMake modules directory
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
message(STATUS "CMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'")

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

#----------------
# Sources
#----------------

# Mach1Encode
set(Mach1EncodeCAPIsrc "../Mach1EncodeCAPI.cpp" "../Mach1EncodeCore.cpp" "../include/cpp/Mach1Encode.cpp" "../Mach1Point3DCore.cpp")
# Mach1Decode
set(Mach1DecodeCAPIsrc "../Mach1DecodeCAPI.cpp" "../Mach1DecodeCore.cpp" "../Mach1Point3DCore.cpp" "../Mach1Point4DCore.cpp" "../include/cpp/Mach1Decode.cpp")
# Mach1DecodePositional
set(Mach1DecodePositionalCAPIsrc "../Mach1DecodePositionalCAPI.cpp" "../Mach1DecodePositionalCore.cpp" "../include/cpp/Mach1DecodePositional.cpp"  "../Mach1Point4DCore.cpp" ${Mach1EncodeCAPIsrc} ${Mach1DecodeCAPIsrc})
# Mach1TranscodeTests
set(Mach1TranscodeCAPIsrc "../Mach1TranscodeCAPI.cpp" "../Mach1TranscodeCore.cpp" "../Mach1EncodeCore.cpp" "../Mach1GenerateCoeffs.cpp" "../deps/M1DSP/M1DSPFilters.cpp" "../deps/M1DSP/M1DSPUtilities.cpp" "../Mach1Point3DCore.cpp" "../Mach1Point4DCore.cpp" "../include/cpp/Mach1Transcode.cpp")

# create the executable
add_executable(Mach1EncodeTests "./Mach1EncodeTests/mach1encode-tests.cpp" ${Mach1EncodeCAPIsrc})
add_executable(Mach1DecodeTests "./Mach1DecodeTests/mach1decode-tests.cpp" ${Mach1DecodeCAPIsrc})
add_executable(Mach1DecodePositionalTests "./Mach1DecodePositionalTests/mach1decodepositional-tests.cpp" ${Mach1DecodePositionalCAPIsrc})
add_executable(Mach1TranscodeTests "./Mach1TranscodeTests/mach1transcode-tests.cpp" ${Mach1TranscodeCAPIsrc})
add_executable(Mach1SpatialTests "./Mach1SpatialTests/mach1spatial-tests.cpp" ${Mach1DecodeCAPIsrc} ${Mach1EncodeCAPIsrc})

#----------------
# Link
#----------------

# include headers
set(Mach1EncodeCAPIinclude "../" "../include/cpp")
set(Mach1DecodeCAPIinclude "../" "../include/cpp")
set(Mach1DecodePositionalCAPIinclude "../" "../include/cpp" "../deps")
set(Mach1TranscodeCAPIinclude "../" "../include/cpp" "../deps" "../../public")
set(Mach1CAPIinclude "../" "../include/cpp" "../deps")

target_include_directories(Mach1EncodeTests PRIVATE "./include" ${Mach1EncodeCAPIinclude})
target_include_directories(Mach1DecodeTests PRIVATE "./include" ${Mach1DecodeCAPIinclude})
target_include_directories(Mach1DecodePositionalTests PRIVATE "./include" ${Mach1DecodePositionalCAPIinclude})
target_include_directories(Mach1TranscodeTests PRIVATE "./include" ${Mach1TranscodeCAPIinclude})
target_include_directories(Mach1SpatialTests PRIVATE "./include" ${Mach1CAPIinclude})

#----------------
# Install
#----------------

message(STATUS "CMAKE_INSTALL_PREFIX='${CMAKE_INSTALL_PREFIX}'")

install(TARGETS Mach1EncodeTests DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1EncodeTests)" )
install(TARGETS Mach1DecodeTests DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1DecodeTests)" )
install(TARGETS Mach1DecodePositionalTests DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1DecodePositionalTests)" )
install(TARGETS Mach1TranscodeTests DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1TranscodeTests)" )
install(TARGETS Mach1SpatialTests DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1SpatialTests)" )

#-------------------------------------------------------------------------------