#!/bin/bash

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/beaglebone/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/linux-arm/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/linux/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/xcode/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/xcode-bundle/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/osx-10-13/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86_64/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/windows-x86/include"
	rsync -rc "include/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/windows-x86_64/include"
	rsync -rc "include/java/jni/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-arm64-v8a/include"
	rsync -rc "include/java/jni/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-armeabi-v7a/include"
	rsync -rc "include/java/jni/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-x86/include"
	rsync -rc "include/java/jni/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-x86_64/include"
	rsync -rc "include/js/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/web"
	rsync -rc "include/swift/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/ios/include"
	echo "Copied includes to mach1spatial-libs release dirs"
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi