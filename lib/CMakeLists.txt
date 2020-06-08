cmake_minimum_required(VERSION 2.8.8)
project(Mach1SpatialLibraries)
set(Mach1SpatialLibraries_VERSION_MAJOR 1)
set(Mach1SpatialLibraries_VERSION_MINOR 1)
set(Mach1SpatialLibraries_VERSION ${Mach1SpatialLibraries_VERSION_MAJOR}.${Mach1SpatialLibraries_VERSION_MINOR})

# SET API VERSIONS
set(ENCODE_VERSION "1.2.0.0")
set(DECODE_VERSION "1.2.0.1")
set(DECODEPOSITIONAL_VERSION "1.2.0.1")
set(TRANSCODE_VERSION "1.2.0.0")

set(Mach1AdditionalInclude "./include")

set(Mach1EncodeCAPIsrc "./Mach1EncodeCAPI.cpp" "./Mach1EncodeCore.cpp" "./Mach1Point3DCore.cpp")
set(Mach1DecodeCAPIsrc "./Mach1DecodeCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
set(Mach1DecodePositionalCAPIsrc "./Mach1DecodePositionalCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1DecodePositionalCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
set(Mach1TranscodeCAPIsrc "./Mach1TranscodeCore.cpp" "./Mach1TranscodeCAPI.cpp" "./Mach1Point3DCore.cpp" "./Mach1GenerateCoeffs.cpp" "${Mach1AdditionalInclude}/M1DSP/M1DSPFilters.cpp" "${Mach1AdditionalInclude}/M1DSP/M1DSPDynamics.cpp" "${Mach1AdditionalInclude}/M1DSP/M1DSPUtilities.cpp")

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

set_target_properties(Mach1DecodeCAPIstatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)
set_target_properties(Mach1DecodePositionalCAPIstatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)
set_target_properties(Mach1EncodeCAPIstatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)
set_target_properties(Mach1TranscodeCAPIstatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/static/)

target_include_directories(Mach1DecodePositionalCAPIstatic PRIVATE ${Mach1AdditionalInclude})
target_include_directories(Mach1TranscodeCAPIstatic PRIVATE ${Mach1AdditionalInclude})

set_target_properties(Mach1EncodeCAPIstatic PROPERTIES VERSION ${ENCODE_VERSION})
set_target_properties(Mach1DecodeCAPIstatic PROPERTIES VERSION ${DECODE_VERSION})
set_target_properties(Mach1DecodePositionalCAPIstatic PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
set_target_properties(Mach1TranscodeCAPIstatic PROPERTIES VERSION ${TRANSCODE_VERSION})

install(TARGETS Mach1DecodeCAPIstatic DESTINATION lib)
install(TARGETS Mach1DecodePositionalCAPIstatic DESTINATION lib)
install(TARGETS Mach1EncodeCAPIstatic DESTINATION lib)
install(TARGETS Mach1TranscodeCAPIstatic DESTINATION lib)

# UNITY
if(NOT IOS OR TVOS OR WATCHOS)
	add_library(Mach1DecodeCAPIshared SHARED ${Mach1DecodeCAPIsrc})
	add_library(Mach1DecodePositionalCAPIshared SHARED ${Mach1DecodePositionalCAPIsrc})
	add_library(Mach1EncodeCAPIshared SHARED ${Mach1EncodeCAPIsrc})
	add_library(Mach1TranscodeCAPIshared SHARED ${Mach1TranscodeCAPIsrc})

	set_target_properties(Mach1DecodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
	set_target_properties(Mach1DecodePositionalCAPIshared PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
	set_target_properties(Mach1EncodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")
	set_target_properties(Mach1TranscodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1TranscodeCAPI")

	target_include_directories(Mach1DecodePositionalCAPIshared PRIVATE ${Mach1AdditionalInclude})
	target_include_directories(Mach1TranscodeCAPIshared PRIVATE ${Mach1AdditionalInclude})

	set_target_properties(Mach1EncodeCAPIshared PROPERTIES VERSION ${ENCODE_VERSION})
	set_target_properties(Mach1DecodeCAPIshared PROPERTIES VERSION ${DECODE_VERSION})
	set_target_properties(Mach1DecodePositionalCAPIshared PROPERTIES VERSION ${DECODEPOSITIONAL_VERSION})
	set_target_properties(Mach1TranscodeCAPIshared PROPERTIES VERSION ${TRANSCODE_VERSION})

	install(TARGETS Mach1DecodeCAPIshared DESTINATION libUnity)
	install(TARGETS Mach1DecodePositionalCAPIshared DESTINATION libUnity)
	install(TARGETS Mach1EncodeCAPIshared DESTINATION libUnity)
	install(TARGETS Mach1TranscodeCAPIshared DESTINATION libUnity)
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

		target_include_directories(Mach1DecodePositionalCAPIbundle PRIVATE ${Mach1AdditionalInclude})
		target_include_directories(Mach1TranscodeCAPIbundle PRIVATE ${Mach1AdditionalInclude})

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

	target_include_directories(Mach1DecodePositionalCAPIframework PRIVATE ${Mach1AdditionalInclude})
	target_include_directories(Mach1TranscodeCAPIframework PRIVATE ${Mach1AdditionalInclude})

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
