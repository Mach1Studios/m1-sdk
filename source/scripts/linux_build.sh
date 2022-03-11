#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain gcc
rsync -c "_install/gcc/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1DecodeCAPI.a"
rsync -c "_install/gcc/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1EncodeCAPI.a"
rsync -c "_install/gcc/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/linux/lib/libMach1DecodePositionalCAPI.a"
# COPYING FOR OFXMACH1
mkdir -p ../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/linux
rsync -c "../mach1spatial-libs/linux/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/linux/libMach1DecodeCAPI.a"
rsync -c "../mach1spatial-libs/linux/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/linux/libMach1EncodeCAPI.a"
rsync -c "../mach1spatial-libs/linux/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/linux/libMach1TranscodeCAPI.a"
rsync -c "../mach1spatial-libs/linux/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/linux/libMach1DecodePositionalCAPI.a"
# COPYING FOR UNITY
mkdir -p ../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Linux
mkdir -p ../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Linux
rsync -c "_install/gcc/lib-shared" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Linux/"
rsync -c "_install/gcc/lib-shared" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Linux/"
