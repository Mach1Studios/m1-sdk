#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	echo "MACOS: TESTS"
	cmake ./tests -Btests/_builds/xcode -GXcode -DCMAKE_INSTALL_PREFIX=`pwd`/tests/_install/xcode
	cmake --build tests/_builds/xcode --config Release --target install

	echo "RUN TESTS"
	tests/_install/xcode/bin/Mach1EncodeTests
	tests/_install/xcode/bin/Mach1DecodeTests
	tests/_install/xcode/bin/Mach1DecodePositionalTests
	tests/_install/xcode/bin/Mach1SpatialTests
	tests/_install/xcode/bin/Mach1TranscodeTests

	echo "RENDER CHECK#"
	echo "5.1_C -> M1Spatial"
	cd tests/Mach1TranscodeRenderTest
	wget -O https://github.com/Mach1Studios/m1-sdk-dev/raw/feature/prep-ci/source/tests/Mach1TranscodeRenderTest/m1-debug-shortpt-fiveone.wav
	mkdir -p build && cd build
	cmake ../
	cmake --build .
	#TODO: Fix bug preventing render test from running
	#./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial -out-file-chans 0
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi