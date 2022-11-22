#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	echo "LINUX: TESTS"
	cmake ./tests -Btests/_builds/gcc -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc
	cmake --build tests/_builds/gcc --config Release --target install

	echo "RUN POLLY TESTS"
	tests/_install/gcc/bin/Mach1EncodeTests
	tests/_install/gcc/bin/Mach1DecodeTests
	tests/_install/gcc/bin/Mach1DecodePositionalTests
	tests/_install/gcc/bin/Mach1SpatialTests
	tests/_install/gcc/bin/Mach1TranscodeTests
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi