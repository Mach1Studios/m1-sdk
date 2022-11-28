#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	RWD="$PWD" # setup return working directory
	# Switch to each submodule and prepare a tmp branch for pushing and merging safely
	cd ../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI && git fetch && git checkout -b tmp
	cd $RWD
	cd ../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI && git fetch && git checkout -b tmp
	cd $RWD
	cd ../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI && git fetch && git checkout -b tmp
	cd $RWD
	cd ../examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI && git fetch && git checkout -b tmp
	# WEB submodules
	cd $RWD
	cd ../examples/mach1spatial-web/m1-web-spatialaudioplayer && git fetch && git checkout -b tmp
	cd $RWD
	cd ../examples/mach1spatial-nodejs/mach1spatial-decode && git fetch && git checkout -b tmp
	cd $RWD
	cd ../examples/mach1spatial-nodejs/mach1spatial-encode && git fetch && git checkout -b tmp
	cd $RWD
	cd ../examples/mach1spatial-nodejs/mach1spatial-transcode && git fetch && git checkout -b tmp
	# Setup tmp branch for root repo
	cd $RWD
	cd .. && git fetch && git checkout -b tmp
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi
