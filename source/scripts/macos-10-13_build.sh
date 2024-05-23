#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	# MACOS 10-13 BUILDS
	cmake . -B_builds/osx-10-13 -GXcode -DCMAKE_OSX_DEPLOYMENT_TARGET="10.13" -DCMAKE_INSTALL_PREFIX=`pwd`/_install/osx-10-13
	cmake --build _builds/osx-10-13 --config Release --target install
	# Copy and install
	rsync -c "_install/osx-10-13/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/osx-10-13/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/osx-10-13/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/osx-10-13/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1DecodePositionalCAPI.a"
	# Upload built libs
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/osx-10-13/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/osx-10-13/lib --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi