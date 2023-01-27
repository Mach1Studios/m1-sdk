#----------------
# Setup
#----------------

cmake_minimum_required(VERSION 3.1.0)
project(Mach1SpatialLibraries)

if(DEFINED CXX_VERSION STREQUAL 14)
	set(CMAKE_CXX_STANDARD 14)
	message("SETUP: CXX=14")
elseif(DEFINED CXX_VERSION STREQUAL 17)
	set(CMAKE_CXX_STANDARD 17)
	message("SETUP: CXX=17")
else()
	set(CMAKE_CXX_STANDARD 11)
	message("SETUP: CXX=11")
endif()

if(DEFINED BUILD_MACOS_BUNDLE)
	set(BUILD_MACOS_BUNDLE 1)
	message("SETUP: Building .bundle libs")
endif()

if(DEFINED BUILD_SHARED_LIBS)
	set(BUILD_SHARED_LIBS 1)
	message("SETUP: Building shared libs")
endif()

if(DEFINED BUILD_MINIFIED_DECODE)
	set(BUILD_MINIFIED_DECODE 1)
	message("SETUP: Building minified decode API")
endif()

if(DEFINED COMPILE_MT)
	set(COMPILE_MT 1)
	message("SETUP: Building /MT static libs for windows target")
endif()

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: default is Release")
endif()

message(STATUS "CMAKE_VERSION=${CMAKE_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(MSVC OR WIN32)
	# enable MSVC_RUNTIME_LIBRARY target property
	# see https://cmake.org/cmake/help/latest/policy/CMP0091.html
	if(POLICY CMP0091)
  		cmake_policy(SET CMP0091 NEW)
	endif()
endif()

if(APPLE)
	#set(CMAKE_OSX_SYSROOT macosx11.0)
	#set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14)
	set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
	set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
endif()

# Prepend our CMake modules directory
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
message(STATUS "CMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'")

#----------------
# Version
#----------------

set(Mach1SpatialLibraries_VERSION_MAJOR 3)
set(Mach1SpatialLibraries_VERSION_MINOR 0)
set(Mach1SpatialLibraries_VERSION ${Mach1SpatialLibraries_VERSION_MAJOR}.${Mach1SpatialLibraries_VERSION_MINOR})

# SET API VERSIONS
set(ENCODE_VERSION "3.0.0.0")
set(DECODE_VERSION "3.0.1.0")
set(DECODEPOSITIONAL_VERSION "3.0.1.0")
set(TRANSCODE_VERSION "3.0.0.0")

#----------------
# Sources
#----------------

# include headers
set(Mach1AdditionalIncludes "./deps")
set(Mach1TranscodeAdditionalIncludes "../public")

# collect and add source files
set(Mach1EncodeAPI_SOURCE "./Mach1EncodeCAPI.cpp" "./Mach1EncodeCore.cpp" "./Mach1Point3DCore.cpp")
set(Mach1DecodeAPI_SOURCE "./Mach1DecodeCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
set(Mach1DecodePositionalAPI_SOURCE "./Mach1DecodePositionalCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1DecodePositionalCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
set(Mach1TranscodeAPI_SOURCE "./Mach1TranscodeCAPI.cpp" "./Mach1TranscodeCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1EncodeCore.cpp" "./Mach1GenerateCoeffs.cpp" "${Mach1AdditionalIncludes}/M1DSP/M1DSPFilters.cpp" "${Mach1AdditionalIncludes}/M1DSP/M1DSPDynamics.cpp" "${Mach1AdditionalIncludes}/M1DSP/M1DSPUtilities.cpp" "${Mach1AdditionalIncludes}/xml/pugixml.cpp" "${Mach1AdditionalIncludes}/yaml/Yaml.cpp" "./Mach1AudioTimelineCore.cpp" "./Mach1AudioTimelineCAPI.cpp")

# create the library
add_library(Mach1DecodeAPI STATIC ${Mach1DecodeAPI_SOURCE})
add_library(Mach1DecodePositionalAPI STATIC ${Mach1DecodePositionalAPI_SOURCE})
add_library(Mach1EncodeAPI STATIC ${Mach1EncodeAPI_SOURCE})
add_library(Mach1TranscodeAPI STATIC ${Mach1TranscodeAPI_SOURCE})

#----------------
# Compiler flags
#----------------

set(COMMON_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS ${COMMON_FLAGS} "-fvisibility=hidden")

if(APPLE)
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

message(STATUS "CMAKE_CXX_COMPILER_VERSION='${CMAKE_CXX_COMPILER_VERSION}'")
message(STATUS "CMAKE_CXX_FLAGS='${CMAKE_CXX_FLAGS}'")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG='${CMAKE_CXX_FLAGS_DEBUG}'")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE='${CMAKE_CXX_FLAGS_RELEASE}'")
message(STATUS "CMAKE_CXX_COMPILE_OBJECT='${CMAKE_CXX_COMPILE_OBJECT}'")

if(MSVC OR WIN32)
	if(COMPILE_MT)
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

if(IOS)
	string(COMPARE EQUAL "$ENV{POLLY_IOS_DEVELOPMENT_TEAM}" "" _is_empty)
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
	set_target_properties(Mach1DecodeAPI PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
	set_target_properties(Mach1DecodePositionalAPI PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
	set_target_properties(Mach1EncodeAPI PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
	set_target_properties(Mach1TranscodeAPI PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")

	set_target_properties(Mach1DecodeAPI PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
	set_target_properties(Mach1DecodePositionalAPI PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
	set_target_properties(Mach1EncodeAPI PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
	set_target_properties(Mach1TranscodeAPI PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
endif()

set_target_properties(Mach1EncodeAPI PROPERTIES VERSION ${ENCODE_VERSION})
set_target_properties(Mach1DecodeAPI PROPERTIES VERSION ${DECODE_VERSION})
set_target_properties(Mach1DecodePositionalAPI PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
set_target_properties(Mach1TranscodeAPI PROPERTIES VERSION ${TRANSCODE_VERSION})

set_target_properties(Mach1DecodeAPI PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
set_target_properties(Mach1DecodePositionalAPI PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
set_target_properties(Mach1EncodeAPI PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
set_target_properties(Mach1TranscodeAPI PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")

set_target_properties(Mach1EncodeAPI PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
set_target_properties(Mach1DecodeAPI PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
set_target_properties(Mach1DecodePositionalAPI PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
set_target_properties(Mach1TranscodeAPI PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)

#----------------
# Link
#----------------

target_include_directories(Mach1DecodePositionalAPI PRIVATE ${Mach1AdditionalIncludes})
target_include_directories(Mach1TranscodeAPI PRIVATE ${Mach1AdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})

# MINIFIED MACH1DECODE for ARM/LINUX
if(BUILD_MINIFIED_DECODE)
	set(Mach1DecodeMINAPI_SOURCE "./Mach1DecodeMINCAPI.cpp" "./Mach1DecodeMINCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
	add_library(Mach1DecodeMINAPI STATIC ${Mach1DecodeMINAPI_SOURCE})
	set_target_properties(Mach1DecodeMINAPI PROPERTIES OUTPUT_NAME "Mach1Decode-minifiedCAPI")
	set_target_properties(Mach1DecodeMINAPI PROPERTIES VERSION ${DECODE_VERSION})
	set_target_properties(Mach1DecodeMINAPI PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
	install(TARGETS Mach1DecodeMINAPI DESTINATION lib)
endif()

# UNITY / SHARED libs
if(BUILD_SHARED_LIBS)
	add_library(Mach1DecodeCAPIshared SHARED ${Mach1DecodeAPI_SOURCE})
	add_library(Mach1DecodePositionalCAPIshared SHARED ${Mach1DecodePositionalAPI_SOURCE})
	add_library(Mach1EncodeCAPIshared SHARED ${Mach1EncodeAPI_SOURCE})
	add_library(Mach1TranscodeCAPIshared SHARED ${Mach1TranscodeAPI_SOURCE})

	target_include_directories(Mach1DecodePositionalCAPIshared PRIVATE ${Mach1AdditionalIncludes})
	target_include_directories(Mach1TranscodeCAPIshared PRIVATE ${Mach1AdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})

	set_target_properties(Mach1DecodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
	set_target_properties(Mach1DecodePositionalCAPIshared PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
	set_target_properties(Mach1EncodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
	set_target_properties(Mach1TranscodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")

	set_target_properties(Mach1EncodeCAPIshared PROPERTIES VERSION ${ENCODE_VERSION})
	set_target_properties(Mach1DecodeCAPIshared PROPERTIES VERSION ${DECODE_VERSION})
	set_target_properties(Mach1DecodePositionalCAPIshared PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
	set_target_properties(Mach1TranscodeCAPIshared PROPERTIES VERSION ${TRANSCODE_VERSION})

	install(TARGETS Mach1DecodeCAPIshared DESTINATION lib-shared)
	install(TARGETS Mach1DecodePositionalCAPIshared DESTINATION lib-shared)
	install(TARGETS Mach1EncodeCAPIshared DESTINATION lib-shared)
	install(TARGETS Mach1TranscodeCAPIshared DESTINATION lib-shared)
endif()

if(BUILD_MACOS_BUNDLE)
	# bundles for Unity
	add_library(Mach1DecodeCAPIbundle MODULE ${Mach1DecodeAPI_SOURCE})
	add_library(Mach1DecodePositionalCAPIbundle MODULE ${Mach1DecodePositionalAPI_SOURCE})
	add_library(Mach1EncodeCAPIbundle MODULE ${Mach1EncodeAPI_SOURCE})
	add_library(Mach1TranscodeCAPIbundle MODULE ${Mach1TranscodeAPI_SOURCE})

	target_include_directories(Mach1DecodePositionalCAPIbundle PRIVATE ${Mach1AdditionalIncludes})
	target_include_directories(Mach1TranscodeCAPIbundle PRIVATE ${Mach1AdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})

	set_target_properties(Mach1DecodeCAPIbundle PROPERTIES BUNDLE TRUE)
	set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES BUNDLE TRUE)
	set_target_properties(Mach1EncodeCAPIbundle PROPERTIES BUNDLE TRUE)
	set_target_properties(Mach1TranscodeCAPIbundle PROPERTIES BUNDLE TRUE)

	set_target_properties(Mach1DecodeCAPIbundle PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
	set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
	set_target_properties(Mach1EncodeCAPIbundle PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
	set_target_properties(Mach1TranscodeCAPIbundle PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")

	set_target_properties(Mach1DecodeCAPIbundle PROPERTIES SUFFIX ".bundle")
	set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES SUFFIX ".bundle")
	set_target_properties(Mach1EncodeCAPIbundle PROPERTIES SUFFIX ".bundle")
	set_target_properties(Mach1TranscodeCAPIbundle PROPERTIES SUFFIX ".bundle")

	set_target_properties(Mach1EncodeCAPIbundle PROPERTIES VERSION ${ENCODE_VERSION})
	set_target_properties(Mach1DecodeCAPIbundle PROPERTIES VERSION ${DECODE_VERSION})
	set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
	set_target_properties(Mach1TranscodeCAPIbundle PROPERTIES VERSION ${TRANSCODE_VERSION})

	install(TARGETS Mach1DecodeCAPIbundle DESTINATION libBundle)
	install(TARGETS Mach1DecodePositionalCAPIbundle DESTINATION libBundle)
	install(TARGETS Mach1EncodeCAPIbundle DESTINATION libBundle)
	install(TARGETS Mach1TranscodeCAPIbundle DESTINATION libBundle)
endif()

if(IOS)
	# STATIC
	add_library(Mach1DecodeCAPIframework STATIC ${Mach1DecodeAPI_SOURCE})
	add_library(Mach1DecodePositionalCAPIframework STATIC ${Mach1DecodePositionalAPI_SOURCE})
	add_library(Mach1EncodeCAPIframework STATIC ${Mach1EncodeAPI_SOURCE})
	add_library(Mach1TranscodeCAPIframework STATIC ${Mach1TranscodeAPI_SOURCE})

	target_include_directories(Mach1DecodePositionalCAPIframework PRIVATE ${Mach1AdditionalIncludes})
	target_include_directories(Mach1TranscodeCAPIframework PRIVATE ${Mach1AdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})

	set_target_properties(Mach1DecodeCAPIframework PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
	set_target_properties(Mach1DecodePositionalCAPIframework PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
	set_target_properties(Mach1EncodeCAPIframework PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
	set_target_properties(Mach1TranscodeCAPIframework PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")

	set_target_properties(Mach1DecodeCAPIframework PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
	set_target_properties(Mach1DecodePositionalCAPIframework PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
	set_target_properties(Mach1EncodeCAPIframework PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
	set_target_properties(Mach1TranscodeCAPIframework PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")

	set_target_properties(Mach1DecodeCAPIframework PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
	set_target_properties(Mach1DecodePositionalCAPIframework PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
	set_target_properties(Mach1EncodeCAPIframework PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
	set_target_properties(Mach1TranscodeCAPIframework PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")

	set_target_properties(Mach1DecodeCAPIframework PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)
	set_target_properties(Mach1DecodePositionalCAPIframework PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)
	set_target_properties(Mach1EncodeCAPIframework PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)
	set_target_properties(Mach1TranscodeCAPIframework PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)

	set_target_properties(Mach1EncodeCAPIframework PROPERTIES VERSION ${ENCODE_VERSION})
	set_target_properties(Mach1DecodeCAPIframework PROPERTIES VERSION ${DECODE_VERSION})
	set_target_properties(Mach1DecodePositionalCAPIframework PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
	set_target_properties(Mach1TranscodeCAPIframework PROPERTIES VERSION ${TRANSCODE_VERSION})

	option(BUILD_SHARED_LIBS "..." ON)
	set_target_properties(
		Mach1DecodeCAPIframework
		PROPERTIES
		FRAMEWORK TRUE
	)
	set_target_properties(
		Mach1DecodePositionalCAPIframework
		PROPERTIES
		FRAMEWORK TRUE
	)
	set_target_properties(
		Mach1EncodeCAPIframework
		PROPERTIES
		FRAMEWORK TRUE
	)
	set_target_properties(
		Mach1TranscodeCAPIframework
		PROPERTIES
		FRAMEWORK TRUE
	)

	install(TARGETS Mach1DecodeCAPIframework DESTINATION Frameworks)
	install(TARGETS Mach1DecodePositionalCAPIframework DESTINATION Frameworks)
	install(TARGETS Mach1EncodeCAPIframework DESTINATION Frameworks)
	install(TARGETS Mach1TranscodeCAPIframework DESTINATION Frameworks)
endif()

#----------------
# Install
#----------------

message(STATUS "CMAKE_INSTALL_PREFIX='${CMAKE_INSTALL_PREFIX}'")
install(TARGETS Mach1DecodeAPI DESTINATION lib)
install(TARGETS Mach1DecodePositionalAPI DESTINATION lib)
install(TARGETS Mach1EncodeAPI DESTINATION lib)
install(TARGETS Mach1TranscodeAPI DESTINATION lib)

#-------------------------------------------------------------------------------