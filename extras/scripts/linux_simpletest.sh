#!/bin/bash

if [[ "$PWD" == *extras/scripts ]]
then
	echo "Script called from correct path: $PWD"
	echo "LINUX: TESTS"
	cmake ../../ -B../../_builds/gcc
	cmake --build ../../_builds/gcc

	echo "RUN TESTS"
	../../_builds/gcc/tests/Debug/Mach1EncodeTests || { echo 'Mach1Encode API test failed...' ; exit 1; }
	../../_builds/gcc/tests/Debug/Mach1DecodeTests || { echo 'Mach1Decode API test failed...' ; exit 1; }
	../../_builds/gcc/tests/Debug/Mach1DecodePositionalTests || { echo 'Mach1DecodePositional API test failed...' ; exit 1; }
	../../_builds/gcc/tests/Debug/Mach1TranscodeTests || { echo 'Mach1Transcode API test failed...' ; exit 1; }
	../../_builds/gcc/tests/Debug/Mach1SpatialTests || { echo 'Additional API tests failed...' ; exit 1; }
else
	echo "ERROR: Script called from wrong directory: $PWD" ; exit 1;
fi