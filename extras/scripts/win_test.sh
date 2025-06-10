#!/bin/bash

if [[ "$PWD" == *extras/scripts ]]
then
	echo "Script called from correct path: $PWD"
	echo "WIN: TESTS"
	cmake  ../../ -B../../_builds/win -G "Visual Studio 16 2019" -A x64
	cmake --build ../../_builds/win -- //nologo //verbosity:quiet //clp:ErrorsOnly

	echo "RUN TESTS"
	echo " " | ../../_builds/win/tests/Debug/Mach1EncodeTests.exe
	echo " " | ../../_builds/win/tests/Debug/Mach1DecodeTests.exe
	echo " " | ../../_builds/win/tests/Debug/Mach1DecodePositionalTests.exe
	echo " " | ../../_builds/win/tests/Debug/Mach1SpatialTests.exe
	echo " " | ../../_builds/win/tests/Debug/Mach1TranscodeTests.exe
else
	echo "ERROR: Script called from wrong directory: $PWD" ; exit 1;
fi