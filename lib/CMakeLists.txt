cmake_minimum_required(VERSION 2.8.8)
project(Mach1Library)
set(Mach1Library_VERSION_MAJOR 1)
set(Mach1Library_VERSION_MINOR 0)
set(Mach1Library_VERSION_PATCH 0)
set(Mach1Library_VERSION_TWEAK 3)

set(Mach1DecodeCAPIsrc "./Mach1DecodeCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
set(Mach1DecodePositionalCAPIsrc "./Mach1DecodePositionalCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1DecodePositionalCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
set(Mach1EncodeCAPIsrc "./Mach1EncodeCAPI.cpp" "./Mach1EncodeCore.cpp")
set(Mach1DecodePositionalCAPIinclude "./include")

add_library(Mach1DecodeCAPIstatic STATIC ${Mach1DecodeCAPIsrc})
add_library(Mach1DecodePositionalCAPIstatic STATIC ${Mach1DecodePositionalCAPIsrc})
add_library(Mach1EncodeCAPIstatic STATIC ${Mach1EncodeCAPIsrc})

set_target_properties(Mach1DecodeCAPIstatic PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
set_target_properties(Mach1DecodePositionalCAPIstatic PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
set_target_properties(Mach1EncodeCAPIstatic PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")

target_include_directories(Mach1DecodePositionalCAPIstatic PRIVATE ${Mach1DecodePositionalCAPIinclude})

install(TARGETS Mach1DecodeCAPIstatic DESTINATION libSTATIC)
install(TARGETS Mach1DecodePositionalCAPIstatic DESTINATION libSTATIC)
install(TARGETS Mach1EncodeCAPIstatic DESTINATION libSTATIC)

if(NOT IOS)
	add_library(Mach1DecodeCAPIshared SHARED ${Mach1DecodeCAPIsrc})
	add_library(Mach1DecodePositionalCAPIshared SHARED ${Mach1DecodePositionalCAPIsrc})
	add_library(Mach1EncodeCAPIshared SHARED ${Mach1EncodeCAPIsrc})
	
	set_target_properties(Mach1DecodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
	set_target_properties(Mach1DecodePositionalCAPIshared PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
	set_target_properties(Mach1EncodeCAPIshared PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")

	target_include_directories(Mach1DecodePositionalCAPIshared PRIVATE ${Mach1DecodePositionalCAPIinclude})

	install(TARGETS Mach1DecodeCAPIshared DESTINATION lib)
	install(TARGETS Mach1DecodePositionalCAPIshared DESTINATION lib)
	install(TARGETS Mach1EncodeCAPIshared DESTINATION lib)
endif()

if(APPLE)
	# bundles for Unity macOS
	add_library(Mach1DecodeCAPIbundle SHARED ${Mach1DecodeCAPIsrc})
	add_library(Mach1DecodePositionalCAPIbundle SHARED ${Mach1DecodePositionalCAPIsrc})
	add_library(Mach1EncodeCAPIbundle SHARED ${Mach1EncodeCAPIsrc})
	
	set_target_properties(Mach1DecodeCAPIbundle PROPERTIES BUNDLE TRUE)
	set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES BUNDLE TRUE)
	set_target_properties(Mach1EncodeCAPIbundle PROPERTIES BUNDLE TRUE)

	set_target_properties(Mach1DecodeCAPIbundle PROPERTIES OUTPUT_NAME "Mach1DecodeCAPI")
	set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPI")
	set_target_properties(Mach1EncodeCAPIbundle PROPERTIES OUTPUT_NAME "Mach1EncodeCAPI")

	set_target_properties(Mach1DecodeCAPIbundle PROPERTIES SUFFIX ".bundle")
	set_target_properties(Mach1DecodePositionalCAPIbundle PROPERTIES SUFFIX ".bundle")
	set_target_properties(Mach1EncodeCAPIbundle PROPERTIES SUFFIX ".bundle")

	target_include_directories(Mach1DecodePositionalCAPIBundle PRIVATE ${Mach1DecodePositionalCAPIinclude})

	install(TARGETS Mach1DecodeCAPIbundle DESTINATION lib)
	install(TARGETS Mach1DecodePositionalCAPIbundle DESTINATION lib)
	install(TARGETS Mach1EncodeCAPIbundle DESTINATION lib)
endif()
