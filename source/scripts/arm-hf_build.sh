#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	# ARMHF BUILD for Beaglebone
	cmake . -B_builds/linux-gcc-armhf -DCMAKE_TOOLCHAIN_FILE=./cmake/armhf-toolchain.cmake -DCMAKE_INSTALL_PREFIX=`pwd`/_install/linux-gcc-armhf
	cmake --build _builds/linux-gcc-armhf --config Release --target install
	rsync -c "_install/linux-gcc-armhf/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/linux-gcc-armhf/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/linux-gcc-armhf/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/linux-gcc-armhf/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1DecodePositionalCAPI.a"
	# Upload built libs
	aws s3 sync _install/linux-gcc-armhf/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/beaglebone/lib --cache-control no-cache --metadata-directive REPLACE
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi