cmake_minimum_required(VERSION 2.8.8)
project(Mach1DecodeLibrary)

if(IOS)
	add_library(Mach1DecodeCAPI "./Mach1DecodeCore.cpp" "./Mach1DecodeCAPI.cpp")
else()
	add_library(Mach1DecodeCAPI SHARED "./Mach1DecodeCore.cpp" "./Mach1DecodeCAPI.cpp")
endif()

install(TARGETS Mach1DecodeCAPI DESTINATION lib)
 