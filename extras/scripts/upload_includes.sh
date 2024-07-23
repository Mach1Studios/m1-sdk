#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"

	# Upload includes
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/beaglebone/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/linux-arm/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/xcode/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/linux/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/osx-10-13/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/windows-x86/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/cpp/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/windows-x86_64/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/java/jni/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-arm64-v8a/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/java/jni/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-armeabi-v7a/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/java/jni/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/java/jni/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86_64/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/js/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/web --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	aws s3 sync include/swift/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/ios/include --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	echo "Copied includes to mach1spatial-libs upload dirs"
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi