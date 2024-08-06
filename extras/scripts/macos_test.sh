#!/bin/bash

if [[ "$PWD" == *extras/scripts ]]
then
	echo "Script called from correct path: $PWD"
	echo "MACOS: TESTS"
	cmake ../../ -B../../_builds/xcode -GXcode
	cmake --build ../../_builds/xcode

	echo "RUN TESTS"
	../../_builds/xcode/tests/Debug/Mach1EncodeTests || { echo 'Mach1Encode API test failed...' ; exit 1; }
	../../_builds/xcode/tests/Debug/Mach1DecodeTests || { echo 'Mach1Decode API test failed...' ; exit 1; }
	../../_builds/xcode/tests/Debug/Mach1DecodePositionalTests || { echo 'Mach1DecodePositional API test failed...' ; exit 1; }
	../../_builds/xcode/tests/Debug/Mach1TranscodeTests || { echo 'Mach1Transcode API test failed...' ; exit 1; }
	../../_builds/xcode/tests/Debug/Mach1SpatialTests || { echo 'Additional API tests failed...' ; exit 1; }

	echo "RENDER CHECK#"
	echo "5.1_C -> M1Spatial"
	wget -P ../../tests/Mach1TranscodeRenderTest https://media.githubusercontent.com/media/Mach1Studios/m1-sdk-dev/develop/source/tests/Mach1TranscodeRenderTest/m1-debug-shortpt-fiveone.wav?token=ABVV4WPEM4POB55JFFRSGMLFUBSCK
	../../_builds/xcode/tests/Debug/spatial-transcode-render -in-file "../../tests/Mach1TranscodeRenderTest/m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../../tests/Mach1TranscodeRenderTest/m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial-8 -out-file-chans 0 || { echo 'Mach1Transcode API render test failed...' ; exit 1; }
else
	echo "ERROR: Script called from wrong directory: $PWD" ; exit 1;
fi