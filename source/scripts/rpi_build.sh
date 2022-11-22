#!/bin/bash


if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	# RPI CROSS COMPILE BUILDS
	cmake . -B_builds/raspberrypi2-cxx11 -DRASPBERRY_VERSION=2 -DCMAKE_TOOLCHAIN_FILE=./cmake/rpi-toolchain.cmake -DCMAKE_INSTALL_PREFIX=`pwd`/_install/raspberrypi2-cxx11
	cmake --build _builds/raspberrypi2-cxx11 --config Release --target install
	rsync -c "_install/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a"
	# rsync -c "_install/raspberrypi2-cxx11/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1Decode-minifiedCAPI.a"
	cmake . -B_builds/raspberrypi3-cxx11 -DRASPBERRY_VERSION=3 -DCMAKE_TOOLCHAIN_FILE=./cmake/rpi-toolchain.cmake -DCMAKE_INSTALL_PREFIX=`pwd`/_install/raspberrypi3-cxx11
	cmake --build _builds/raspberrypi3-cxx11 --config Release --target install
	rsync -c "_install/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a"
	# rsync -c "_install/raspberrypi3-cxx11/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1Decode-minifiedCAPI.a"
	# Upload built libs
	aws s3 sync _install/raspberrypi2-cxx11/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync _install/raspberrypi3-cxx11/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib --cache-control no-cache --metadata-directive REPLACE
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi