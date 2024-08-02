if(NOT SKIP_DECODEPOSITIONAL_API)
    set(Mach1DecodePositionalAdditionalIncludes "glm")
    target_include_directories(${PROJECT_NAME} PUBLIC ${Mach1DecodePositionalAdditionalIncludes})
endif()

if(NOT SKIP_TRANSCODE_API)
    set(Mach1TranscodeAdditionalIncludes "yaml/yaml" "pugixml/src" "glm" "M1DSP/M1DSPFilters.cpp" "M1DSP/M1DSPDynamics.cpp" "M1DSP/M1DSPUtilities.cpp" "pugixml/src/pugixml.cpp" "yaml/yaml/Yaml.cpp")
    target_include_directories(${PROJECT_NAME} PUBLIC ${Mach1TranscodeAdditionalIncludes})
endif()