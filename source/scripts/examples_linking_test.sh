#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	echo "RUN LINKING TESTS ON CURRENT DEVICE"
	cd ../examples/mach1spatial-c/commandline/spatial-decode-example
	mkdir -p build && cd build
	cmake ../
	cmake --build .
	cd ../examples/mach1spatial-c/commandline/spatial-encode-example
	mkdir -p build && cd build
	cmake ../
	cmake --build .
	cd ../examples/mach1spatial-c/commandline/spatial-decodepositional-example
	mkdir -p build && cd build
	cmake ../
	cmake --build .
	cd ../examples/mach1spatial-c/commandline/spatial-transcode-example
	mkdir -p build && cd build
	cmake ../
	cmake --build .
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi