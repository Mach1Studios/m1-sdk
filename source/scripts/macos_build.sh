#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source
# MACOS BUILDS
cmake . -B_builds/xcode -GXcode -DBUILD_SHARED_LIBS=1 -DBUILD_MACOS_BUNDLE=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/xcode
cmake --build _builds/xcode --config Release --target install
rsync -c "_install/xcode/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a"
rsync -c "_install/xcode/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a"
rsync -c "_install/xcode/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/xcode/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a"
# Upload built libs
aws s3 sync _install/xcode/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/xcode/lib --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE