cmake_minimum_required(VERSION 3.19.3)

if(MSVC OR WIN32)
	# enable MSVC_RUNTIME_LIBRARY target property
	# see https://cmake.org/cmake/help/latest/policy/CMP0091.html
	if(POLICY CMP0091)
  		cmake_policy(SET CMP0091 NEW)
	endif()
endif()

project(Mach1SpatialLibraries)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")

if(APPLE)
	#set(CMAKE_OSX_SYSROOT macosx11.0)
	#set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14)
	set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
	set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
endif()

set(Mach1SpatialLibraries_VERSION_MAJOR 2)
set(Mach1SpatialLibraries_VERSION_MINOR 0)
set(Mach1SpatialLibraries_VERSION ${Mach1SpatialLibraries_VERSION_MAJOR}.${Mach1SpatialLibraries_VERSION_MINOR})

# SET API VERSIONS
set(ENCODE_VERSION "2.0.0.0")
set(DECODE_VERSION "2.0.1.0")
set(DECODEPOSITIONAL_VERSION "2.0.0.0")
set(TRANSCODE_VERSION "2.1.0.0")

set(Mach1AdditionalIncludes "./deps")
set(Mach1TranscodeAdditionalIncludes "../public")

set(Mach1EncodeCAPIsrc "./Mach1EncodeCAPI.cpp" "./Mach1EncodeCore.cpp" "./Mach1Point3DCore.cpp")
set(Mach1DecodeCAPIsrc "./Mach1DecodeCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
set(Mach1DecodePositionalCAPIsrc "./Mach1DecodePositionalCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1DecodePositionalCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
set(Mach1TranscodeCAPIsrc "./Mach1TranscodeCore.cpp" "./Mach1TranscodeCAPI.cpp" "./Mach1Point3DCore.cpp" "./Mach1GenerateCoeffs.cpp" "${Mach1AdditionalIncludes}/M1DSP/M1DSPFilters.cpp" "${Mach1AdditionalIncludes}/M1DSP/M1DSPDynamics.cpp" "${Mach1AdditionalIncludes}/M1DSP/M1DSPUtilities.cpp" "${Mach1AdditionalIncludes}/xml/pugixml.cpp" "${Mach1AdditionalIncludes}/yaml/Yaml.cpp" "./Mach1AudioTimelineCore.cpp" "./Mach1AudioTimelineCAPI.cpp")

# STATIC
add_library(Mach1DecodeCAPIstatic STATIC ${Mach1DecodeCAPIsrc})
add_library(Mach1DecodePositionalCAPIstatic STATIC ${Mach1DecodePositionalCAPIsrc})
add_library(Mach1EncodeCAPIstatic STATIC ${Mach1EncodeCAPIsrc})
add_library(Mach1TranscodeCAPIstatic STATIC ${Mach1TranscodeCAPIsrc})

if(IOS)
	set_target_properties(Mach1DecodeCAPIstatic PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
	set_target_properties(Mach1DecodePositionalCAPIstatic PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
	set_target_properties(Mach1EncodeCAPIstatic PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")
	set_target_properties(Mach1TranscodeCAPIstatic PROPERTIES CMAKE_XCODE_ATTRIBUTE_BITCODE_GENERATION_MODE "bitcode")

	set_target_properties(Mach1DecodeCAPIstatic PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
	set_target_properties(Mach1DecodePositionalCAPIstatic PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
	set_target_properties(Mach1EncodeCAPIstatic PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
	set_target_properties(Mach1TranscodeCAPIstatic PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")
endif()

set_target_properties(Mach1DecodeCAPIstatic PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
set_target_properties(Mach1DecodePositionalCAPIstatic PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
set_target_properties(Mach1EncodeCAPIstatic PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
set_target_properties(Mach1TranscodeCAPIstatic PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")

target_include_directories(Mach1DecodePositionalCAPIstatic PRIVATE ${Mach1AdditionalIncludes})
target_include_directories(Mach1TranscodeCAPIstatic PRIVATE ${Mach1AdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})

set_target_properties(Mach1EncodeCAPIstatic PROPERTIES VERSION ${ENCODE_VERSION})
set_target_properties(Mach1DecodeCAPIstatic PROPERTIES VERSION ${DECODE_VERSION})
set_target_properties(Mach1DecodePositionalCAPIstatic PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
set_target_properties(Mach1TranscodeCAPIstatic PROPERTIES VERSION ${TRANSCODE_VERSION})

set_target_properties(Mach1EncodeCAPIstatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
set_target_properties(Mach1DecodeCAPIstatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
set_target_properties(Mach1DecodePositionalCAPIstatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
set_target_properties(Mach1TranscodeCAPIstatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)

if(MSVC OR WIN32)
	if(COMPILE_MT)
    	add_compile_options(
        	$<$<CONFIG:>:/MT>
        	$<$<CONFIG:Debug>:/MTd>
        	$<$<CONFIG:Release>:/MT>
    	)
    	set_property(TARGET Mach1DecodeCAPIstatic PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    	set_property(TARGET Mach1DecodePositionalCAPIstatic PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    	set_property(TARGET Mach1EncodeCAPIstatic PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    	set_property(TARGET Mach1TranscodeCAPIstatic PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    	#set_property(TARGET Mach1DecodeMINCAPIstatic PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
	endif()
endif()

# MINIFIED MACH1DECODE for ARM/LINUX
if(NOT APPLE OR NOT WIN32 OR NOT MSVC OR NOT ANDROID)
	set(Mach1DecodeMINCAPIsrc "./Mach1DecodeMINCAPI.cpp" "./Mach1DecodeMINCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
	add_library(Mach1DecodeMINCAPIstatic STATIC ${Mach1DecodeMINCAPIsrc})
	set_target_properties(Mach1DecodeMINCAPIstatic PROPERTIES OUTPUT_NAME "Mach1Decode-minifiedCAPI")
	set_target_properties(Mach1DecodeMINCAPIstatic PROPERTIES VERSION ${DECODE_VERSION})
	set_target_properties(Mach1DecodeMINCAPIstatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static)
	install(TARGETS Mach1DecodeMINCAPIstatic DESTINATION lib)
endif()

install(TARGETS Mach1DecodeCAPIstatic DESTINATION lib)
install(TARGETS Mach1DecodePositionalCAPIstatic DESTINATION lib)
install(TARGETS Mach1EncodeCAPIstatic DESTINATION lib)
install(TARGETS Mach1TranscodeCAPIstatic DESTINATION lib)

# UNITY / SHARED libs
if (UNIX OR WIN32 OR ANDROID)
	if(NOT IOS OR NOT TVOS OR NOT WATCHOS)
		add_library(Mach1DecodeCAPIshared SHARED ${Mach1DecodeCAPIsrc})
		add_library(Mach1DecodePositionalCAPIshared SHARED ${Mach1DecodePositionalCAPIsrc})
		add_library(Mach1EncodeCAPIshared SHARED ${Mach1EncodeCAPIsrc})
		add_library(Mach1TranscodeCAPIshared SHARED ${Mach1TranscodeCAPIsrc})

		set_target_properties(Mach1DecodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
		set_target_properties(Mach1DecodePositionalCAPIshared PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
		set_target_properties(Mach1EncodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
		set_target_properties(Mach1TranscodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")

		target_include_directories(Mach1DecodePositionalCAPIshared PRIVATE ${Mach1AdditionalIncludes})
		target_include_directories(Mach1TranscodeCAPIshared PRIVATE ${Mach1AdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})

		set_target_properties(Mach1EncodeCAPIshared PROPERTIES VERSION ${ENCODE_VERSION})
		set_target_properties(Mach1DecodeCAPIshared PROPERTIES VERSION ${DECODE_VERSION})
		set_target_properties(Mach1DecodePositionalCAPIshared PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
		set_target_properties(Mach1TranscodeCAPIshared PROPERTIES VERSION ${TRANSCODE_VERSION})

		install(TARGETS Mach1DecodeCAPIshared DESTINATION lib-shared)
		install(TARGETS Mach1DecodePositionalCAPIshared DESTINATION lib-shared)
		install(TARGETS Mach1EncodeCAPIshared DESTINATION lib-shared)
		install(TARGETS Mach1TranscodeCAPIshared DESTINATION lib-shared)
	endif()
endif()

if(APPLE)
	if(NOT IOS OR TVOS OR WATCHOS)
		# bundles for Unity
		add_library(Mach1DecodeCAPIbundle SHARED ${Mach1DecodeCAPIsrc})
		add_library(Mach1DecodePositionalCAPIbundle SHARED ${Mach1DecodePositionalCAPIsrc})
		add_library(Mach1EncodeCAPIbundle SHARED ${Mach1EncodeCAPIsrc})
		add_library(Mach1TranscodeCAPIbundle SHARED ${Mach1TranscodeCAPIsrc})

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

		target_include_directories(Mach1DecodePositionalCAPIbundle PRIVATE ${Mach1AdditionalIncludes})
		target_include_directories(Mach1TranscodeCAPIbundle PRIVATE ${Mach1AdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})

		set_target_properties(Mach1EncodeCAPIbundle PROPERTIES VERSION ${ENCODE_VERSION})
		set_target_properties(Mach1DecodeCAPIbundle PROPERTIES VERSION ${DECODE_VERSION})
		set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
		set_target_properties(Mach1TranscodeCAPIbundle PROPERTIES VERSION ${TRANSCODE_VERSION})

		install(TARGETS Mach1DecodeCAPIbundle DESTINATION libBundle)
		install(TARGETS Mach1DecodePositionalCAPIbundle DESTINATION libBundle)
		install(TARGETS Mach1EncodeCAPIbundle DESTINATION libBundle)
		install(TARGETS Mach1TranscodeCAPIbundle DESTINATION libBundle)
	endif()
endif()

if(IOS)
	# STATIC
	add_library(Mach1DecodeCAPIframework STATIC ${Mach1DecodeCAPIsrc})
	add_library(Mach1DecodePositionalCAPIframework STATIC ${Mach1DecodePositionalCAPIsrc})
	add_library(Mach1EncodeCAPIframework STATIC ${Mach1EncodeCAPIsrc})
	add_library(Mach1TranscodeCAPIframework STATIC ${Mach1TranscodeCAPIsrc})

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

	target_include_directories(Mach1DecodePositionalCAPIframework PRIVATE ${Mach1AdditionalIncludes})
	target_include_directories(Mach1TranscodeCAPIframework PRIVATE ${Mach1AdditionalIncludes} ${Mach1TranscodeAdditionalIncludes})

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
