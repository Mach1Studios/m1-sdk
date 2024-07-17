#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	cmake . -B_builds/gcc-armhf-neon-vfpv4 -DCMAKE_TOOLCHAIN_FILE=./cmake/armhf-neon-vfpv4-toolchain.cmake -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc-armhf-neon-vfpv4
	cmake --build _builds/gcc-armhf-neon-vfpv4 --config Release --target install
	rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1DecodePositionalCAPI.a"
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi