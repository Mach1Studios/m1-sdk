#!/bin/bash

#replace all binaries in examples
echo "### Change Dir ###"
cd "/Volumes/git/m1-sdk/lib"

echo "### Move and replace bins ###"
echo "### Replacing Xcode/macOS ###"
yes | cp -rf _install/xcode ../binaries/
yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/osx/libMach1DecodeCAPI.a"
yes | cp -rf "_install/xcode/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/osx/libMach1EncodeCAPI.a"
yes | cp -rf "_install/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/osx/libMach1DecodePositionalCAPI.a"

yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/cs/Plugins/macOS/libMach1DecodeCAPI.bundle"
yes | cp -rf "_install/xcode/lib/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/cs/Plugins/macOS/libMach1EncodeCAPI.bundle"
yes | cp -rf "_install/xcode/lib/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/cs/Plugins/macOS/libMach1DecodePositionalCAPI.bundle"

yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle"
yes | cp -rf "_install/xcode/lib/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1EncodeCAPI.bundle"
yes | cp -rf "_install/xcode/lib/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/cs/Plugins/macOS/libMach1DecodePositionalCAPI.bundle"

yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodeCAPI.a"
yes | cp -rf "_install/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodePositionalCAPI.a"

echo "### Replacing iOS ###"
yes | cp -rf _install/ios ../binaries/
yes | cp -rf "_install/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/cs/Plugins/iOS/libMach1DecodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodeCAPI.a"
echo "### Moving iOS ###"
yes | cp -rf "_install/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/ios/libMach1DecodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/ios/libMach1EncodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/ios/libMach1DecodePositionalCAPI.a"

echo "### Replacing Android x86 & armaebi-v7a ###"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx" "../binaries/"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/AndroidNativeSound/app/src/main/jniLibs/armeabi-v7a/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/cs/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/android/armeabi-v7a/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/android/armeabi-v7a/libMach1EncodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/android/armeabi-v7a/libMach1DecodePositionalCAPI.a"

yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx" "../binaries/"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/AndroidNativeSound/app/src/main/jniLibs/x86/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/cs/Plugins/Android/x86/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/android/x86/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/android/x86/libMach1EncodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/android/x86/libMach1DecodePositionalCAPI.a"

echo "### Copying all successful bins to /Binaries ###"
yes | cp -rf "_install/" "../binaries/"
yes | cp -rf "external/" "../include/"

echo "### Copying for ofxMach1 ###"
yes | cp -rf "external/cpp/Mach1Decode.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.h"
yes | cp -rf "external/cpp/Mach1Decode.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.cpp"
yes | cp -rf "external/cpp/Mach1Encode.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.h"
yes | cp -rf "external/cpp/Mach1Encode.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.cpp"

yes | cp -rf "external/cpp/Mach1DecodeCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/include/Mach1DecodeCAPI.h"
yes | cp -rf "external/cpp/Mach1EncodeCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/include/Mach1EncodeCAPI.h"
yes | cp -rf "external/cpp/Mach1Point3D.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/include/Mach1Point3D.h"
yes | cp -rf "external/cpp/Mach1Point4D.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/include/Mach1Point4D.h"

echo "### Copying ofxMach1 to dev local ###"
yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../openFrameworks/addons/"
yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../openFrameworks-git/addons/"
yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../of_v0.9.8_osx_release/addons/"
yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../of_v0.10.0_osx_release/addons/"
yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../of_v0.10.1_osx_release/addons/"

echo "### NUMBERS OF SUCCESSFUL BUILDS:"
cd _logs/polly/
grep -rl "Installing:" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l

echo "### ERRORS CAUGHT:"
grep -rl "error" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
grep -rl "Error" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
grep -rl "failed" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
grep -rl "FAILED" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l