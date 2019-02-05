cmake_minimum_required(VERSION 2.8.8)
project(Mach1Library)
set(Mach1Library_VERSION_MAJOR 1)
set(Mach1Library_VERSION_MINOR 0)
set(Mach1Library_VERSION_PATCH 0)
set(Mach1Library_VERSION_TWEAK 2)

if(IOS)
	add_library(Mach1DecodeCAPI "./Mach1DecodeCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
	add_library(Mach1DecodePositionalCAPI  "./Mach1DecodePositionalCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1DecodePositionalCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
	add_library(Mach1EncodeCAPI "./Mach1EncodeCAPI.cpp" "./Mach1EncodeCore.cpp")
else()
	add_library(Mach1DecodeCAPI SHARED "./Mach1DecodeCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
	add_library(Mach1DecodePositionalCAPI SHARED "./Mach1DecodePositionalCAPI.cpp" "./Mach1DecodeCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1DecodePositionalCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
	add_library(Mach1EncodeCAPI SHARED "./Mach1EncodeCAPI.cpp" "./Mach1EncodeCore.cpp")

	# bundle for Unity macOS
	add_library(Mach1DecodePositionalCAPIBundle SHARED "./Mach1DecodePositionalCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1DecodePositionalCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
	set_target_properties(Mach1DecodePositionalCAPIBundle PROPERTIES BUNDLE TRUE)
	set_target_properties(Mach1DecodePositionalCAPIBundle PROPERTIES OUTPUT_NAME "Mach1DecodePositionalCAPIBundle")
	set_target_properties(Mach1DecodePositionalCAPIBundle PROPERTIES SUFFIX ".bundle")
endif()

target_include_directories(Mach1DecodePositionalCAPI PRIVATE "./include")
if(NOT IOS)
	target_include_directories(Mach1DecodePositionalCAPIBundle PRIVATE "./include")
endif()

install(TARGETS Mach1DecodeCAPI DESTINATION lib)
install(TARGETS Mach1EncodeCAPI DESTINATION lib)
install(TARGETS Mach1DecodePositionalCAPI DESTINATION lib)
if(NOT IOS)
	install(TARGETS Mach1DecodePositionalCAPIBundle DESTINATION lib)
endif()

add_library(Mach1DecodeCAPI_Static "./Mach1DecodeCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
add_library(Mach1DecodePositionalCAPI_Static "./Mach1DecodePositionalCAPI.cpp" "./Mach1DecodeCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1DecodePositionalCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
add_library(Mach1EncodeCAPI_Static "./Mach1EncodeCAPI.cpp" "./Mach1EncodeCore.cpp")

target_include_directories(Mach1DecodePositionalCAPI_Static PRIVATE "./include")

install(TARGETS Mach1DecodeCAPI_Static DESTINATION libStatic)
install(TARGETS Mach1EncodeCAPI_Static DESTINATION libStatic)
install(TARGETS Mach1DecodePositionalCAPI_Static DESTINATION libStatic)