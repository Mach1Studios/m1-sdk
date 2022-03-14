#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source
# MACOS BUILDS
${POLLY_SOURCE_DIR}/bin/polly --clear --install --config Release --toolchain xcode
rsync -c "_install/xcode/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a"
rsync -c "_install/xcode/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a"
rsync -c "_install/xcode/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/xcode/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a"
# Upload built libs
aws s3 sync "_install/xcode/lib/libMach1DecodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/xcode/lib/libMach1EncodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/xcode/lib/libMach1TranscodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/xcode/lib/libMach1DecodePositionalCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
# # COPYING FOR UE
# rsync -c "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodeCAPI.a"
# rsync -c "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodePositionalCAPI.a"
# # COPYING FOR UNITY
# rsync -c "_install/xcode/libBundle/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle"
# rsync -c "_install/xcode/libBundle/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1EncodeCAPI.bundle"
# rsync -c "_install/xcode/libBundle/libMach1TranscodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1TranscodeCAPI.bundle"
# rsync -c "_install/xcode/libBundle/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodePositionalCAPI.bundle"
# # COPYING FOR OFXMACH1
# rsync -c "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodeCAPI.a"
# rsync -c "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1EncodeCAPI.a"
# rsync -c "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1TranscodeCAPI.a"
# rsync -c "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodePositionalCAPI.a"
