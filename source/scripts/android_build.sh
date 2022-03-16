#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-clang-libcxx
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-64-clang-libcxx
cd ${TRAVIS_BUILD_DIR}/source
mv _install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx _install/android-armeabi-v7a
mv _install/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx _install/android-arm64-v8a
mv _install/android-ndk-r16b-api-21-x86-clang-libcxx _install/android-x86
mv _install/android-ndk-r16b-api-21-x86-64-clang-libcxx _install/android-x86_64
# INSTALL ANDROID LIBS
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
# Upload built libs
aws s3 sync _install/android-armeabi-v7a/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-armeabi-v7a/lib --exclude "*-minifiedCAPI*" --cache-control no-cache
aws s3 sync _install/android-arm64-v8a/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-arm64-v8a/lib --exclude "*-minifiedCAPI*" --cache-control no-cache
aws s3 sync _install/android-x86/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86/lib --exclude "*-minifiedCAPI*" --cache-control no-cache
aws s3 sync _install/android-x86_64/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86_64/lib --exclude "*-minifiedCAPI*" --cache-control no-cache
