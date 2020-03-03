cmake_minimum_required(VERSION 2.8.8)
project(Mach1Tests)

set(Mach1EncodeCAPIsrc "../Mach1EncodeCAPI.cpp" "../Mach1EncodeCore.cpp" "../external/cpp/Mach1Encode.cpp")
set(Mach1EncodeCAPIinclude "../" "../external/cpp")

add_executable(Mach1EncodeTest "./Mach1EncodeTest/mach1encode-test.cpp" ${Mach1EncodeCAPIsrc})
target_include_directories(Mach1EncodeTest PRIVATE "./include" ${Mach1EncodeCAPIinclude})

install(TARGETS Mach1EncodeTest DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1EncodeTest)" )

set(Mach1DecodeCAPIsrc "../Mach1DecodeCAPI.cpp" "../Mach1DecodeCore.cpp" "../external/cpp/Mach1Decode.cpp")
set(Mach1DecodeCAPIinclude "../" "../external/cpp")

add_executable(Mach1DecodeTest "./Mach1DecodeTest/mach1decode-test.cpp" ${Mach1DecodeCAPIsrc})
target_include_directories(Mach1DecodeTest PRIVATE "./include" ${Mach1DecodeCAPIinclude})

install(TARGETS Mach1DecodeTest DESTINATION bin)
install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1DecodeTest)" )

# set(Mach1DecodePositionalCAPIsrc "../Mach1DecodePositionalCAPI.cpp" "../Mach1DecodePositionalCore.cpp" "../external/cpp/Mach1DecodePositional.cpp")
# set(Mach1DecodePositionalCAPIinclude "../" "../external/cpp")

# add_executable(Mach1DecodePositionalTest "./Mach1DecodePositionalTest/mach1decodepositional-test.cpp" ${Mach1DecodePositionalCAPIsrc})
# target_include_directories(Mach1DecodePositionalTest PRIVATE "./include" ${Mach1DecodePositionalCAPIinclude})

# install(TARGETS Mach1DecodePositionalTest DESTINATION bin)
# install(CODE "execute_process(COMMAND ${CMAKE_INSTALL_PREFIX}/bin/Mach1DecodePositionalTest)" )