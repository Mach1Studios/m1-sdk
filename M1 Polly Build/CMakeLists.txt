cmake_minimum_required(VERSION 2.8.8)
project(M1DSPAlgorithmsLibrary)

include(GenerateExportHeader)

add_library(M1DSPAlgorithmsCAPI SHARED "./M1DSPAlgorithms.cpp" "./M1DSPAlgorithmsCAPI.cpp")

generate_export_header(M1DSPAlgorithmsCAPI)
install(TARGETS M1DSPAlgorithmsCAPI DESTINATION lib)
 