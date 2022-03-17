cmake_minimum_required(VERSION 3.19.3)
project(Mach1Tests)
if (APPLE)
	#set(CMAKE_OSX_SYSROOT macosx11.0)
	#set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14)
	set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
	set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
endif()

message("CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")
message("CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message("CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message("CMAKE_CROSSCOMPILING: ${CMAKE_CROSSCOMPILING}")

# Mach1Encode
set(Mach1EncodeCAPIsrc "../Mach1EncodeCAPI.cpp" "../Mach1EncodeCore.cpp" "../external/cpp/Mach1Encode.cpp" "../Mach1Point3DCore.cpp")
set(Mach1EncodeCAPIinclude "../" "../external/cpp")

add_executable(Mach1EncodeTest "./Mach1EncodeTest/mach1encode-test.cpp" ${Mach1EncodeCAPIsrc})
target_include_directories(Mach1EncodeTest PRIVATE "./include" ${Mach1EncodeCAPIinclude})

install(TARGETS Mach1EncodeTest DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1EncodeTest)" )

# Mach1Decode

set(Mach1DecodeCAPIsrc "../Mach1DecodeCAPI.cpp" "../Mach1DecodeCore.cpp" "../Mach1Point3DCore.cpp" "../Mach1Point4DCore.cpp" "../external/cpp/Mach1Decode.cpp")
set(Mach1DecodeCAPIinclude "../" "../external/cpp")

add_executable(Mach1DecodeTest "./Mach1DecodeTest/mach1decode-test.cpp" ${Mach1DecodeCAPIsrc})
target_include_directories(Mach1DecodeTest PRIVATE "./include" ${Mach1DecodeCAPIinclude})

install(TARGETS Mach1DecodeTest DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1DecodeTest)" )

# Mach1DecodePositional

set(Mach1DecodePositionalCAPIsrc "../Mach1DecodePositionalCAPI.cpp" "../Mach1DecodePositionalCore.cpp" "../external/cpp/Mach1DecodePositional.cpp"  "../Mach1Point4DCore.cpp" ${Mach1EncodeCAPIsrc} ${Mach1DecodeCAPIsrc})
set(Mach1DecodePositionalCAPIinclude "../" "../external/cpp" "../include")

add_executable(Mach1DecodePositionalTest "./Mach1DecodePositionalTest/mach1decodepositional-test.cpp" ${Mach1DecodePositionalCAPIsrc})
target_include_directories(Mach1DecodePositionalTest PRIVATE "./include" ${Mach1DecodePositionalCAPIinclude})

install(TARGETS Mach1DecodePositionalTest DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1DecodePositionalTest)" )

# Mach1SpatialTests

set(Mach1DecodeCAPIsrc "../Mach1DecodeCAPI.cpp" "../Mach1DecodeCore.cpp" "../Mach1Point3DCore.cpp" "../Mach1Point4DCore.cpp" "../external/cpp/Mach1Decode.cpp")
set(Mach1EncodeCAPIsrc "../Mach1EncodeCAPI.cpp" "../Mach1EncodeCore.cpp" "../external/cpp/Mach1Encode.cpp")
set(Mach1CAPIinclude "../" "../external/cpp" "../include")

add_executable(Mach1SpatialTests "./Mach1SpatialTests/mach1spatial-tests.cpp" ${Mach1DecodeCAPIsrc} ${Mach1EncodeCAPIsrc})
target_include_directories(Mach1SpatialTests PRIVATE "./include" ${Mach1CAPIinclude})

install(TARGETS Mach1SpatialTests DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1SpatialTests)" )

# Mach1TranscodeTests

set(Mach1TranscodeCAPIsrc "../Mach1TranscodeCAPI.cpp" "../Mach1TranscodeCore.cpp" "../Mach1EncodeCore.cpp" "../Mach1GenerateCoeffs.cpp" "../include/M1DSP/M1DSPFilters.cpp" "../include/M1DSP/M1DSPUtilities.cpp" "../Mach1Point3DCore.cpp" "../Mach1Point4DCore.cpp" "../external/cpp/Mach1Transcode.cpp")
set(Mach1CAPIinclude "../" "../external/cpp" "../include" "../../public")

add_executable(Mach1TranscodeTests "./Mach1TranscodeTests/mach1transcode-tests.cpp" ${Mach1TranscodeCAPIsrc})
target_include_directories(Mach1TranscodeTests PRIVATE "./include" ${Mach1CAPIinclude})

install(TARGETS Mach1TranscodeTests DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1TranscodeTests)" )
