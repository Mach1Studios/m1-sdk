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
aws s3 sync "_install/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-arm64-v8a/lib/libMach1DecodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-arm64-v8a/lib/libMach1EncodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-arm64-v8a/lib/libMach1EncodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-arm64-v8a/lib/libMach1TranscodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-arm64-v8a/lib/libMach1TranscodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-x86/lib/libMach1DecodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-x86/lib/libMach1EncodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-x86/lib/libMach1TranscodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-x86/lib/libMach1DecodePositionalCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-x86_64/lib/libMach1DecodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86_64/lib/libMach1DecodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-x86_64/lib/libMach1EncodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86_64/lib/libMach1EncodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-x86_64/lib/libMach1TranscodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86_64/lib/libMach1TranscodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/android-x86_64/lib/libMach1DecodePositionalCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/android-x86_64/lib/libMach1DecodePositionalCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
# # COPYING FOR UNITY
# rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so" 
# rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1EncodeCAPI.so"
# rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1TranscodeCAPI.so"
# rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodePositionalCAPI.so"
# rsync -c "_install/android-arm64-v8a/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodeCAPI.so"
# rsync -c "_install/android-arm64-v8a/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1EncodeCAPI.so"
# rsync -c "_install/android-arm64-v8a/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1TranscodeCAPI.so"
# rsync -c "_install/android-arm64-v8a/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodePositionalCAPI.so"
# rsync -c "_install/android-x86/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
# rsync -c "_install/android-x86/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1EncodeCAPI.so"
# rsync -c "_install/android-x86/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1TranscodeCAPI.so"
# rsync -c "_install/android-x86/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodePositionalCAPI.so"
# rsync -c "_install/android-x86_64/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86_64/libMach1DecodeCAPI.so"
# rsync -c "_install/android-x86_64/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86_64/libMach1EncodeCAPI.so"
# rsync -c "_install/android-x86_64/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86_64/libMach1TranscodeCAPI.so"
# rsync -c "_install/android-x86_64/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86_64/libMach1DecodePositionalCAPI.so"
# rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so"
# rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1EncodeCAPI.so"
# rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1TranscodeCAPI.so"
# rsync -c "_install/android-armeabi-v7a/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodePositionalCAPI.so"
# rsync -c "_install/android-arm64-v8a/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodeCAPI.so"
# rsync -c "_install/android-arm64-v8a/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1EncodeCAPI.so"
# rsync -c "_install/android-arm64-v8a/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1TranscodeCAPI.so"
# rsync -c "_install/android-arm64-v8a/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodePositionalCAPI.so"
# rsync -c "_install/android-x86/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
# rsync -c "_install/android-x86/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1EncodeCAPI.so"
# rsync -c "_install/android-x86/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1TranscodeCAPI.so"
# rsync -c "_install/android-x86/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1DecodePositionalCAPI.so"
# rsync -c "_install/android-x86_64/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86_64/libMach1DecodeCAPI.so"
# rsync -c "_install/android-x86_64/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86_64/libMach1EncodeCAPI.so"
# rsync -c "_install/android-x86_64/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86_64/libMach1TranscodeCAPI.so"
# rsync -c "_install/android-x86_64/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86_64/libMach1DecodePositionalCAPI.so"
# # COPYING FOR UE
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1EncodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1TranscodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodePositionalCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1EncodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1TranscodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodePositionalCAPI.a"
# # COPYING FOR OFXMACH1
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1EncodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1TranscodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodePositionalCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1EncodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1TranscodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodePositionalCAPI.a"
# # COPYING FOR JITPACK
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1DecodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1EncodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1TranscodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1DecodePositionalCAPI.a"
# rsync -c "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1DecodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-arm64-v8a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1EncodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-arm64-v8a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1TranscodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1DecodePositionalCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1DecodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1EncodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1TranscodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1DecodePositionalCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86_64/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1DecodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86_64/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1EncodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86_64/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1TranscodeCAPI.a"
# rsync -c "../mach1spatial-libs/android-x86_64/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1DecodePositionalCAPI.a"        
# cp -rf include/cpp/*CAPI.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
# cp -rf include/cpp/*D.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
# cp -rf include/java/jni/java/com/mach1/spatiallibs/* "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/java/com/mach1/spatiallibs"
# rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/mach1-decode-example/src/main/jniLibs/armeabi-v7a/libMach1DecodeCAPI.a"
# cp -rf include/cpp/Mach1Decode.cpp "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
# cp -rf include/cpp/Mach1Decode.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
# cp -rf include/cpp/Mach1DecodeCAPI.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
# cp -rf include/cpp/*D.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/mach1-decode-example/src/main/cpp"