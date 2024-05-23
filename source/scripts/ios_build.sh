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
	# IOS BUILDS
	cmake . -B_builds/ios-11-3-dep-9-0-device-bitcode -GXcode -T buildsystem=1 -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=9.3 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/ios-11-3-dep-9-0-device-bitcode -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO -DCMAKE_IOS_INSTALL_COMBINED=YES
	cmake --build _builds/ios-11-3-dep-9-0-device-bitcode --config Release --target install
	mkdir -p "../mach1spatial-libs/ios/lib"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a"
	mkdir -p "../mach1spatial-libs/ios/Frameworks"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/Mach1DecodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/Mach1DecodeCAPI.framework"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/Mach1EncodeCAPI.framework" "../mach1spatial-libs/ios/FrameworksFrameworks/Mach1EncodeCAPI.framework"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/Mach1DecodePositionalCAPI.framework" "../mach1spatial-libs/ios/Frameworks/Mach1DecodePositionalCAPI.framework"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/Mach1TranscodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/Mach1TranscodeCAPI.framework"
	rsync -c include/swift/* ../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes
	# Upload built libs
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/ios-11-3-dep-9-0-device-bitcode/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/ios/lib --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi