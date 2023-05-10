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
	# MACOS BUILDS
	cmake . -B_builds/xcode -GXcode -DBUILD_SHARED_LIBS=1 -DBUILD_MACOS_BUNDLE=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/xcode
	cmake --build _builds/xcode --config Release --target install
	rsync -c "_install/xcode/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/xcode/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/xcode/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/xcode/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a"
	# bundles
	rsync -c "_install/xcode/libBundle/Mach1DecodeCAPI.bundle" "../mach1spatial-libs/xcode-bundle/lib/Mach1DecodeCAPI.bundle"
	rsync -c "_install/xcode/libBundle/Mach1EncodeCAPI.bundle" "../mach1spatial-libs/xcode-bundle/lib/Mach1EncodeCAPI.bundle"
	rsync -c "_install/xcode/libBundle/Mach1TranscodeCAPI.bundle" "../mach1spatial-libs/xcode-bundle/lib/Mach1TranscodeCAPI.bundle"
	rsync -c "_install/xcode/libBundle/Mach1DecodePositionalCAPI.bundle" "../mach1spatial-libs/xcode-bundle/lib/Mach1DecodePositionalCAPI.bundle"
	# Upload built libs
	if [[ $upload_artifacts == "ON" ]]
	then
		# static
		aws s3 sync _install/xcode/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/xcode/lib --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
		# dynamic
		# aws s3 sync _install/xcode/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/xcode/lib-shared --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
		# bundle
		aws s3 sync _install/xcode-bundle/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/xcode-bundle/lib --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi