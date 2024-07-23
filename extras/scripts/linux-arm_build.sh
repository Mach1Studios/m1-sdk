#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	cmake . -B_builds/gcc -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc
	cmake --build _builds/gcc --config Release --target install
	rsync -c "_install/gcc/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1DecodePositionalCAPI.a"
	# Upload built libs
	aws s3 sync _install/gcc/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/linux-arm/lib --cache-control no-cache --metadata-directive REPLACE
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
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi