#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	# ARM CORTEXM0 BUILD
	cmake . -B_builds/gcc-arm-cortexm0-cxx14 -DCMAKE_TOOLCHAIN_FILE=./cmake/cortex-m0-toolchain.cmake -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc-arm-cortexm0-cxx14 -DCXX_VERSION=14
	cmake --build _builds/gcc-arm-cortexm0-cxx14 --config Release --target install
	rsync -c "_install/gcc-arm-cortexm0-cxx14/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/gcc-arm-cortexm0-cxx14/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/gcc-arm-cortexm0-cxx14/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/gcc-arm-cortexm0-cxx14/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/gcc-arm-cortexm0-cxx14/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1Decode-minifiedCAPI.a"
	# Upload built libs
	aws s3 sync _install/gcc-arm-cortexm0-cxx14/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib --cache-control no-cache --metadata-directive REPLACE
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi