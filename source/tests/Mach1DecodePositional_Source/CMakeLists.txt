#----------------
# Setup
#----------------

cmake_minimum_required(VERSION 3.1.0)
project(Mach1DecodePositional_Source)
set(CMAKE_CXX_STANDARD 11)

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

# Mach1DecodePositional
set(Mach1DecodeCAPIsrc "../../Mach1DecodeCAPI.cpp" "../../Mach1DecodeCore.cpp" "../../include/cpp/Mach1Decode.cpp")
set(Mach1DecodePositionalCAPIsrc "../../Mach1DecodePositionalCAPI.cpp" "../../Mach1DecodePositionalCore.cpp" "../../Mach1Point3D.cpp" "../../Mach1Point4D.cpp" "../../include/cpp/Mach1DecodePositional.cpp" ${Mach1DecodeCAPIsrc})

# create the executable
add_executable(Mach1DecodePositional_Source "../../../examples/mach1spatial-c/commandline/spatial-decodepositional-example/src/main.cpp" ${Mach1DecodePositionalCAPIsrc})

#----------------
# Link
#----------------

# include headers
set(Mach1CAPIinclude "../../" "../../include/cpp" "../../deps")
set(Mach1DecodePositionalCAPIinclude ${Mach1CAPIinclude} "../../deps/glm")
target_include_directories(Mach1DecodePositional_Source PRIVATE ${Mach1DecodePositionalCAPIinclude})

#----------------
# Install
#----------------

message(STATUS "CMAKE_INSTALL_PREFIX='${CMAKE_INSTALL_PREFIX}'")

install(TARGETS Mach1DecodePositional_Source DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1DecodePositional_Source)" )

#-------------------------------------------------------------------------------