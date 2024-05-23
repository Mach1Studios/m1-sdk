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
	echo "Compiling Static Android libs..."
	cmake . -B_builds/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DBUILD_SHARED_LIBS=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx
	cmake --build _builds/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx --config Release --target install
	cmake . -B_builds/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DBUILD_SHARED_LIBS=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
	cmake --build _builds/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx --config Release --target install
	cmake . -B_builds/android-ndk-r16b-api-21-x86-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=x86 -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DBUILD_SHARED_LIBS=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-x86-clang-libcxx
	cmake --build _builds/android-ndk-r16b-api-21-x86-clang-libcxx --config Release --target install
	cmake . -B_builds/android-ndk-r16b-api-21-x86-64-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=x86_64 -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DBUILD_SHARED_LIBS=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-x86-64-clang-libcxx
	cmake --build _builds/android-ndk-r16b-api-21-x86-64-clang-libcxx --config Release --target install
	cd ${TRAVIS_BUILD_DIR}/source
	mv _install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx _install/android-armeabi-v7a
	mv _install/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx _install/android-arm64-v8a
	mv _install/android-ndk-r16b-api-21-x86-clang-libcxx _install/android-x86
	mv _install/android-ndk-r16b-api-21-x86-64-clang-libcxx _install/android-x86_64
	# INSTALL STATIC ANDROID LIBS
	rsync -c "_install/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/android-arm64-v8a/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/android-arm64-v8a/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/android-arm64-v8a/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/android-x86/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/android-x86/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/android-x86/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/android-x86/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/android-x86_64/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/android-x86_64/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/android-x86_64/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/android-x86_64/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1DecodePositionalCAPI.a"
	# INSTALL SHARED ANDROID LIBS
	rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1DecodeCAPI.so" "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1DecodeCAPI.so"
	rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1EncodeCAPI.so" "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1EncodeCAPI.so"
	rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1TranscodeCAPI.so" "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1TranscodeCAPI.so"
	rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1DecodePositionalCAPI.so" "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1DecodePositionalCAPI.so"
	rsync -c "_install/android-arm64-v8a/lib-shared/libMach1DecodeCAPI.so" "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1DecodeCAPI.so"
	rsync -c "_install/android-arm64-v8a/lib-shared/libMach1EncodeCAPI.so" "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1EncodeCAPI.so"
	rsync -c "_install/android-arm64-v8a/lib-shared/libMach1TranscodeCAPI.so" "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1TranscodeCAPI.so"
	rsync -c "_install/android-arm64-v8a/lib-shared/libMach1DecodePositionalCAPI.so" "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1DecodePositionalCAPI.so"
	rsync -c "_install/android-x86/lib-shared/libMach1DecodeCAPI.so" "../mach1spatial-libs/android-x86/lib-shared/libMach1DecodeCAPI.so"
	rsync -c "_install/android-x86/lib-shared/libMach1EncodeCAPI.so" "../mach1spatial-libs/android-x86/lib-shared/libMach1EncodeCAPI.so"
	rsync -c "_install/android-x86/lib-shared/libMach1TranscodeCAPI.so" "../mach1spatial-libs/android-x86/lib-shared/libMach1TranscodeCAPI.so"
	rsync -c "_install/android-x86/lib-shared/libMach1DecodePositionalCAPI.so" "../mach1spatial-libs/android-x86/lib-shared/libMach1DecodePositionalCAPI.so"
	rsync -c "_install/android-x86_64/lib-shared/libMach1DecodeCAPI.so" "../mach1spatial-libs/android-x86_64/lib-shared/libMach1DecodeCAPI.so"
	rsync -c "_install/android-x86_64/lib-shared/libMach1EncodeCAPI.so" "../mach1spatial-libs/android-x86_64/lib-shared/libMach1EncodeCAPI.so"
	rsync -c "_install/android-x86_64/lib-shared/libMach1TranscodeCAPI.so" "../mach1spatial-libs/android-x86_64/lib-shared/libMach1TranscodeCAPI.so"
	rsync -c "_install/android-x86_64/lib-shared/libMach1DecodePositionalCAPI.so" "../mach1spatial-libs/android-x86_64/lib-shared/libMach1DecodePositionalCAPI.so"
	# Upload built static libs
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/android-armeabi-v7a/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-armeabi-v7a/lib --exclude "*-minifiedCAPI*" --cache-control no-cache
		aws s3 sync _install/android-arm64-v8a/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-arm64-v8a/lib --exclude "*-minifiedCAPI*" --cache-control no-cache
		aws s3 sync _install/android-x86/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86/lib --exclude "*-minifiedCAPI*" --cache-control no-cache
		aws s3 sync _install/android-x86_64/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86_64/lib --exclude "*-minifiedCAPI*" --cache-control no-cache
		# shared libs
		aws s3 sync _install/android-armeabi-v7a/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-armeabi-v7a/lib-shared --exclude "*-minifiedCAPI*" --cache-control no-cache
		aws s3 sync _install/android-arm64-v8a/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-arm64-v8a/lib-shared --exclude "*-minifiedCAPI*" --cache-control no-cache
		aws s3 sync _install/android-x86/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86/lib-shared --exclude "*-minifiedCAPI*" --cache-control no-cache
		aws s3 sync _install/android-x86_64/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86_64/lib-shared --exclude "*-minifiedCAPI*" --cache-control no-cache
	fi
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi