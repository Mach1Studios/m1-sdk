#!/bin/bash

# RPI CROSS COMPILE BUILDS
cd ${TRAVIS_BUILD_DIR}/source
${POLLY_SOURCE_DIR}/bin/polly --clear --install --config Release --toolchain gcc-armhf-neon-vfpv4
rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1DecodeCAPI.a"
rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1EncodeCAPI.a"
rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1DecodePositionalCAPI.a"
rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1Decode-minifiedCAPI.a"