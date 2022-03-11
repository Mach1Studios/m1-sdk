#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain gcc
rsync -c "_install/gcc/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1DecodeCAPI.a"
rsync -c "_install/gcc/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1EncodeCAPI.a"
rsync -c "_install/gcc/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1DecodePositionalCAPI.a"
# INLINE LINKING TEST (avoiding confusion between linux vs linux-arm)
cd ${TRAVIS_BUILD_DIR}/examples/mach1spatial-c/commandline/spatial-decode-example
mkdir -p build && cd build
g++ -o ./spatial-decode-example ../src/main.cpp ${TRAVIS_BUILD_DIR}/mach1spatial-libs/arm-chipsets/linux-arm/include/Mach1Decode.cpp -I${TRAVIS_BUILD_DIR}/mach1spatial-libs/arm-chipsets/linux-arm/include -L${TRAVIS_BUILD_DIR}/mach1spatial-libs/arm-chipsets/linux-arm/lib -lstdc++ -pthread -lMach1DecodeCAPI
cd ${TRAVIS_BUILD_DIR}/examples/mach1spatial-c/commandline/spatial-encode-example
mkdir -p build && cd build
g++ -o ./spatial-encode-example ../src/main.cpp ${TRAVIS_BUILD_DIR}/mach1spatial-libs/arm-chipsets/linux-arm/include/Mach1Encode.cpp -I${TRAVIS_BUILD_DIR}/mach1spatial-libs/arm-chipsets/linux-arm/include -L${TRAVIS_BUILD_DIR}/mach1spatial-libs/arm-chipsets/linux-arm/lib -lstdc++ -pthread -lMach1EncodeCAPI
cd ${TRAVIS_BUILD_DIR}/examples/mach1spatial-c/commandline/spatial-transcode-example
mkdir -p build && cd build
g++ -o ./spatial-transcode-example ../src/main.cpp ${TRAVIS_BUILD_DIR}/mach1spatial-libs/arm-chipsets/linux-arm/include/Mach1Transcode.cpp -I${TRAVIS_BUILD_DIR}/mach1spatial-libs/arm-chipsets/linux-arm/include -L${TRAVIS_BUILD_DIR}/mach1spatial-libs/arm-chipsets/linux-arm/lib -lstdc++ -pthread -lMach1TranscodeCAPI
