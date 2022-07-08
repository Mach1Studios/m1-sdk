#!/bin/bash

# IOS BUILDS
cd ${TRAVIS_BUILD_DIR}/source
cmake . -B_builds/ios-11-3-dep-9-0-device-bitcode -GXcode -T buildsystem=1 -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=9.3 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/ios-11-3-dep-9-0-device-bitcode -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO -DCMAKE_IOS_INSTALL_COMBINED=YES
cmake --build _builds/ios-11-3-dep-9-0-device-bitcode --config Release --target install
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1DecodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1DecodeCAPI.framework"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1EncodeCAPI.framework" "../mach1spatial-libs/ios/FrameworksFrameworks/libMach1EncodeCAPI.framework"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1DecodePositionalCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1DecodePositionalCAPI.framework"
rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1TranscodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1TranscodeCAPI.framework"
# Upload built libs
aws s3 sync _install/ios-11-3-dep-9-0-device-bitcode/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/ios/lib --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE