#!/bin/bash

while getopts 'uh:' OPTION; do
	case "$OPTION" in
		u)
			upload_artifacts=ON
			echo "Uploading binaries to aws"
			;;
		h)
			echo "script usage: $(basename \$0) [-u] [-h]" >&2
			exit 1
			;;
		?)
			echo "script usage: $(basename \$0) [-u] [-h]" >&2
			exit 1
			;;
	esac
done
# shift "$(($OPTION -1))"

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	XRSDK_PATH=$(xcodebuild -version -sdk xros Path)
	echo "XCode XRSDK_PATH=$XRSDK_PATH"
	# IOS BUILDS
	cmake . -B_builds/visionOS -GXcode -DCMAKE_SYSTEM_NAME=visionOS -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_SYSTEM_PROCESSOR=arm64 -DCMAKE_OSX_SYSROOT=$XRSDK_PATH -DCMAKE_INSTALL_PREFIX=`pwd`/_install/visionOS
	cmake --build _builds/visionOS --config Release --target install
	mkdir -p "../mach1spatial-libs/visionOS/lib"
	rsync -c "_install/visionOS/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/visionOS/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/visionOS/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/visionOS/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/visionOS/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/visionOS/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/visionOS/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/visionOS/lib/libMach1TranscodeCAPI.a"
	rsync -c include/swift/* ../mach1spatial-libs/visionOS/include
	Upload built libs
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/visionOS/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/visionOS/lib --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi