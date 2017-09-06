cmake_minimum_required(VERSION 2.8.8)
project(Mach1DecodeLibrary)

include(GenerateExportHeader)

add_library(Mach1DecodeCAPI SHARED "./Mach1Decode.cpp" "./Mach1DecodeCAPI.cpp")

generate_export_header(Mach1DecodeCAPI)
install(TARGETS Mach1DecodeCAPI DESTINATION lib)
 