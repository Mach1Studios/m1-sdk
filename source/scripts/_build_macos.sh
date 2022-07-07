#!/bin/bash

# Please run from source directory

# MACOS BUILDS
cmake . -B_builds/macos -GXcode -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=ON -DBUILD_MACOS_BUNDLE=1 -DBUILD_SHARED_LIBS=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/macos
cmake --build _builds/macos --config Release --target install
rsync -c "_install/macos/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a"
rsync -c "_install/macos/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a"
rsync -c "_install/macos/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/macos/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a"

#IOS BUILDS
cmake . -B_builds/ios -GXcode -T buildsystem=1 -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -DCMAKE_INSTALL_PREFIX=`pwd`/_install/ios -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=YES -DCMAKE_IOS_INSTALL_COMBINED=YES
cmake --build _builds/ios --config Release --target install
