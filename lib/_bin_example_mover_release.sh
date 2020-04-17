#!/bin/bash

#replace all binaries in github release 
echo "### Change Dir ###"
cd "/Volumes/git/m1-sdk/lib"

echo "### Copying all successful bins to /Binaries ###"
yes | cp -rf "_install/" "../../m1-sdk-release/binaries/"
yes | cp -rf "external/" "../../m1-sdk-release/include/"

echo "### Move and replace bins ###"
echo "### Replacing Xcode/macOS ###"
yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodeCAPI.a"
yes | cp -rf "_install/xcode/lib/libMach1EncodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1EncodeCAPI.a"
yes | cp -rf "_install/xcode/lib/libMach1DecodePositionalCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodePositionalCAPI.a"

echo "### Moving iOS ###"
yes | cp -rf "_install/ios/lib/libMach1DecodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1EncodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1EncodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1DecodePositionalCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodePositionalCAPI.a"

echo "### Replacing Android ###"
yes | cp -rf "_install/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1EncodeCAPI.a"
yes | cp -rf "_install/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodePositionalCAPI.a"
yes | cp -rf "_install/android-x86/lib/libMach1DecodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-x86/lib/libMach1EncodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1EncodeCAPI.a"
yes | cp -rf "_install/android-x86/lib/libMach1DecodePositionalCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodePositionalCAPI.a"

echo "### Copying for ofxMach1 ###"
yes | cp -rf "external/cpp/Mach1Decode.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.h"
yes | cp -rf "external/cpp/Mach1Decode.cpp" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.cpp"
yes | cp -rf "external/cpp/Mach1DecodePositional.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.h"
yes | cp -rf "external/cpp/Mach1DecodePositional.cpp" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.cpp"
yes | cp -rf "external/cpp/Mach1Encode.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.h"
yes | cp -rf "external/cpp/Mach1Encode.cpp" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.cpp"
yes | cp -rf "external/cpp/Mach1DecodeCAPI.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodeCAPI.h"
yes | cp -rf "external/cpp/Mach1DecodePositionalCAPI.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodePositionalCAPI.h"
yes | cp -rf "external/cpp/Mach1EncodeCAPI.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1EncodeCAPI.h"
yes | cp -rf "external/cpp/Mach1Point3D.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point3D.h"
yes | cp -rf "external/cpp/Mach1Point4D.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point4D.h"

echo "### DELETE all libUnity dirs"
cd ../../m1-sdk-release/binaries
find . -type d -name 'libUnity' -print0 | xargs -0 rm -rf
find . -type d -name 'libBundle' -print0 | xargs -0 rm -rf