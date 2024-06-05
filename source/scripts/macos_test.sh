#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	echo "MACOS: TESTS"
	cmake ./tests -Btests/_builds/xcode -GXcode -DCMAKE_INSTALL_PREFIX=`pwd`/tests/_install/xcode
	cmake --build tests/_builds/xcode --config Release --target install

	echo "RUN TESTS"
	tests/_install/xcode/bin/Mach1EncodeTests || { echo 'Mach1Encode API test failed...' ; exit 1; }
	tests/_install/xcode/bin/Mach1DecodeTests || { echo 'Mach1Decode API test failed...' ; exit 1; }
	tests/_install/xcode/bin/Mach1DecodePositionalTests || { echo 'Mach1DecodePositional API test failed...' ; exit 1; }
	tests/_install/xcode/bin/Mach1TranscodeTests || { echo 'Mach1Transcode API test failed...' ; exit 1; }
	tests/_install/xcode/bin/Mach1SpatialTests || { echo 'Additional API tests failed...' ; exit 1; }

	echo "RENDER CHECK#"
	echo "5.1_C -> M1Spatial"
	cd tests/Mach1TranscodeRenderTest
	#wget https://media.githubusercontent.com/media/Mach1Studios/m1-sdk-dev/develop/source/tests/Mach1TranscodeRenderTest/m1-debug-shortpt-fiveone.wav?token=ABVV4WPEM4POB55JFFRSGMLFUBSCK
	cmake ./tests/ -Btests/_builds/xcode -GXcode -DCMAKE_INSTALL_PREFIX=`pwd`/tests/_install/xcode
	cmake --build build
	#build/spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial -out-file-chans 0
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi