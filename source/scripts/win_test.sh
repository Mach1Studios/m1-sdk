#!/bin/bash

if [[ "$PWD" == *source/tests ]]
then
	echo "Script called from correct path: $PWD"
	echo "WIN: TESTS"
	cmake ./tests -Btests/_builds/vs-15-2017 -G "Visual Studio 15 2017" -A x64 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017
	cmake --build tests/_builds/vs-15-2017 --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly

	echo "RUN TESTS"
	echo " " | tests/_install/vs-15-2017/bin/Mach1EncodeTests.exe
	echo " " | tests/_install/vs-15-2017/bin/Mach1DecodeTests.exe
	echo " " | tests/_install/vs-15-2017/bin/Mach1DecodePositionalTests.exe
	echo " " | tests/_install/vs-15-2017/bin/Mach1SpatialTests.exe
	echo " " | tests/_install/vs-15-2017/bin/Mach1TranscodeTests.exe
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi