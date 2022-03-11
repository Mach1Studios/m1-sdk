#!/bin/bash

# ARMHF BUILD for Beaglebone
cd ${TRAVIS_BUILD_DIR}/source
${POLLY_SOURCE_DIR}/bin/polly --clear --install --config Release --toolchain linux-gcc-armhf
rsync -c "_install/linux-gcc-armhf/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1DecodeCAPI.a"
rsync -c "_install/linux-gcc-armhf/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1EncodeCAPI.a"
rsync -c "_install/linux-gcc-armhf/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/linux-gcc-armhf/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1DecodePositionalCAPI.a"
rsync -c "_install/linux-gcc-armhf/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1Decode-minifiedCAPI.a"
