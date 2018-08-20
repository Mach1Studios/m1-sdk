cmake_minimum_required(VERSION 2.8.8)
project(Mach1Library)

if(IOS)
	add_library(Mach1DecodeCAPI "./Mach1DecodeCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
	add_library(Mach1DecodePositionalCAPI  "./Mach1DecodePositionalCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1DecodePositionalCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
	add_library(Mach1EncodeCAPI "./Mach1EncodeCAPI.cpp" "./Mach1EncodeCore.cpp")
else()
	add_library(Mach1DecodeCAPI SHARED "./Mach1DecodeCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
	add_library(Mach1DecodePositionalCAPI SHARED "./Mach1DecodePositionalCAPI.cpp" "./Mach1DecodeCore.cpp" "./Mach1DecodePositionalCore.cpp" "./Mach1Point3DCore.cpp" "./Mach1Point4DCore.cpp")
	add_library(Mach1EncodeCAPI SHARED "./Mach1EncodeCAPI.cpp" "./Mach1EncodeCore.cpp")
endif()

target_include_directories(Mach1DecodePositionalCAPI PRIVATE "./include")
target_link_libraries(Mach1DecodePositionalCAPI Mach1DecodeCAPI)

install(TARGETS Mach1DecodeCAPI DESTINATION lib)
install(TARGETS Mach1EncodeCAPI DESTINATION lib)
install(TARGETS Mach1DecodePositionalCAPI DESTINATION lib)
