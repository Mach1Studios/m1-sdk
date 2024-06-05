#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	echo "LINUX: TESTS"
	cmake ./tests -Btests/_builds/gcc -DCMAKE_INSTALL_PREFIX=`pwd`/tests/_install/gcc
	cmake --build tests/_builds/gcc --config Release --target install

	echo "RUN TESTS"
	tests/_install/gcc/bin/Mach1EncodeTests || { echo 'Mach1Encode API test failed...' ; exit 1; }
	tests/_install/gcc/bin/Mach1DecodeTests || { echo 'Mach1Decode API test failed...' ; exit 1; }
	tests/_install/gcc/bin/Mach1DecodePositionalTests || { echo 'Mach1DecodePositional API test failed...' ; exit 1; }
	tests/_install/gcc/bin/Mach1TranscodeTests || { echo 'Mach1Transcode API test failed...' ; exit 1; }
	tests/_install/gcc/bin/Mach1SpatialTests || { echo 'Additional API tests failed...' ; exit 1; }
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi