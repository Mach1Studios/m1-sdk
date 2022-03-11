#!/bin/bash

# IOS BUILDS
cd ${TRAVIS_BUILD_DIR}/source
mkdir -p ${TRAVIS_BUILD_DIR}/source/_builds/ios-11-3-dep-9-0-device-bitcode/Release-iphoneos
${POLLY_SOURCE_DIR}/bin/polly --install --config Release --toolchain ios-11-3-dep-9-0-device-bitcode --ios-multiarch --ios-combined
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1DecodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1DecodeCAPI.framework"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1EncodeCAPI.framework" "../mach1spatial-libs/ios/FrameworksFrameworks/libMach1EncodeCAPI.framework"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1DecodePositionalCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1DecodePositionalCAPI.framework"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1TranscodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1TranscodeCAPI.framework"
# COPYING FOR UE
rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1EncodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1TranscodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodePositionalCAPI.a"
# COPYING FOR UNITY
rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1EncodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1TranscodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodePositionalCAPI.a"
# COPYING FOR OFXMACH1
rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1EncodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1TranscodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodePositionalCAPI.a"
# COPYING FOR COCOAPODS
rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1EncodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1TranscodeCAPI.a"
rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodePositionalCAPI.a"
cp -rf include/swift/* "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes/"