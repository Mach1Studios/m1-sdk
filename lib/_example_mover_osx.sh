#!/bin/bash

#replace all binaries in examples
echo "### Change Dir ###"
cd "/Volumes/git/m1-sdk/lib"

echo "Built ios-12-1-dep-9-3?"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) 
			yes | cp -rf "_install/ios-12-1-dep-9-3/lib/libMach1DecodeCAPI.a" "_install/ios/lib/libMach1DecodeCAPI.a"
			yes | cp -rf "_install/ios-12-1-dep-9-3/lib/libMach1EncodeCAPI.a" "_install/ios/lib/libMach1EncodeCAPI.a"
			yes | cp -rf "_install/ios-12-1-dep-9-3/lib/libMach1DecodePositionalCAPI.a" "_install/ios/lib/libMach1DecodePositionalCAPI.a"
			rm -rf "_install/ios-12-1-dep-9-3"; break;;
        No ) break;;
    esac
done

echo "### Copying shared libs for Unity ###"
yes | cp -rf "_install/xcode/libBundle/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle"
yes | cp -rf "_install/xcode/libBundle/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1EncodeCAPI.bundle"
yes | cp -rf "_install/xcode/libBundle/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodePositionalCAPI.bundle"

yes | cp -rf "_install/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1EncodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodePositionalCAPI.a"

yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1EncodeCAPI.so"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodePositionalCAPI.so"

yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1EncodeCAPI.so"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodePositionalCAPI.so"

yes | cp -rf "_install/windows-x86_64/libUnity/Mach1DecodeCAPI.dll" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Windows/x86_64/Mach1DecodeCAPI.dll"
yes | cp -rf "_install/windows-x86_64/libUnity/Mach1EncodeCAPI.dll" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Windows/x86_64/Mach1EncodeCAPI.dll"
yes | cp -rf "_install/windows-x86_64/libUnity/Mach1DecodePositionalCAPI.dll" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Windows/x86_64/Mach1DecodePositionalCAPI.dll"

yes | cp -rf "_install/windows-x86/libUnity/Mach1DecodeCAPI.dll" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Windows/x86/Mach1DecodeCAPI.dll"
yes | cp -rf "_install/windows-x86/libUnity/Mach1EncodeCAPI.dll" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Windows/x86/Mach1EncodeCAPI.dll"
yes | cp -rf "_install/windows-x86/libUnity/Mach1DecodePositionalCAPI.dll" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Windows/x86/Mach1DecodePositionalCAPI.dll"

echo "### Copying all successful bins to /Binaries ###"
yes | cp -rf "_install/" "../binaries/"
yes | cp -rf "external/" "../include/"

echo "### Move and replace bins ###"
echo "### Replacing Xcode/macOS ###"
yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodeCAPI.a"
yes | cp -rf "_install/xcode/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1EncodeCAPI.a"
yes | cp -rf "_install/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodePositionalCAPI.a"

yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodeCAPI.a"
yes | cp -rf "_install/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodePositionalCAPI.a"

echo "### Replacing iOS ###"
yes | cp -rf "_install/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1EncodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodePositionalCAPI.a"

echo "### Moving iOS ###"
yes | cp -rf "_install/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1EncodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodePositionalCAPI.a"

echo "### Copying to CocoaPods Repo ###"
yes | cp -rf "_install/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1EncodeCAPI.a"
yes | cp -rf "_install/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodePositionalCAPI.a"
yes | cp -rf external/swift/* "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes/"

echo "### Replacing Android x86 & armaebi-v7a ###"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/AndroidNativeSound/app/src/main/jniLibs/armeabi-v7a/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1EncodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodePositionalCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1EncodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodePositionalCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/AndroidNativeSound/app/src/main/jniLibs/x86/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1EncodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodePositionalCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1EncodeCAPI.a"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodePositionalCAPI.a"

echo "### Copying for ofxMach1 ###"
yes | cp -rf "external/cpp/Mach1Decode.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.h"
yes | cp -rf "external/cpp/Mach1Decode.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.cpp"
yes | cp -rf "external/cpp/Mach1DecodePositional.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.h"
yes | cp -rf "external/cpp/Mach1DecodePositional.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.cpp"
yes | cp -rf "external/cpp/Mach1Encode.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.h"
yes | cp -rf "external/cpp/Mach1Encode.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.cpp"
yes | cp -rf "external/cpp/Mach1DecodeCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodeCAPI.h"
yes | cp -rf "external/cpp/Mach1DecodePositionalCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodePositionalCAPI.h"
yes | cp -rf "external/cpp/Mach1EncodeCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1EncodeCAPI.h"
yes | cp -rf "external/cpp/Mach1Point3D.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point3D.h"
yes | cp -rf "external/cpp/Mach1Point4D.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point4D.h"

echo "### Copying for UE ###"
yes | cp -rf "external/cpp/Mach1DecodeCAPI.h" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/include/Mach1DecodeCAPI.h"
yes | cp -rf "external/cpp/Mach1DecodePositionalCAPI.h" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/include/Mach1DecodePositionalCAPI.h"
yes | cp -rf "external/cpp/Mach1Decode.h" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/Source/Mach1DecodePlugin/Public/Mach1Decode.h"
yes | cp -rf "external/cpp/Mach1Decode.cpp" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/Source/Mach1DecodePlugin/Public/Mach1DecodePositional.h"
yes | cp -rf "external/cpp/Mach1DecodePositional.h" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/Source/Mach1DecodePlugin/Private/Mach1Decode.cpp"
yes | cp -rf "external/cpp/Mach1DecodePositional.cpp" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/Source/Mach1DecodePlugin/Private/Mach1DecodePositional.cpp"

echo "### Copying for Unity ###"
yes | cp -rf external/c#/* "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/"

echo "### Copying ofxMach1 to dev local ###"
yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../openFrameworks/addons/"
yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../openFrameworks-git/addons/"
yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../of_v0.9.8_osx_release/addons/"
yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../of_v0.10.0_osx_release/addons/"
yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../of_v0.10.1_osx_release/addons/"

echo "### DELETE all libUnity dirs"
cd ../binaries
find . -type d -name 'libUnity' -print0 | xargs -0 rm -rf
find . -type d -name 'libBundle' -print0 | xargs -0 rm -rf

echo "### NUMBERS OF SUCCESSFUL BUILDS:"
cd ../lib/_logs/polly/
grep -rl "Installing:" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l

echo "### ERRORS CAUGHT:"
grep -rl "error" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
grep -rl "Error" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
grep -rl "failed" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
grep -rl "FAILED" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l

cd ../../
echo "Execute _bin_example_mover_release.sh?"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) 
			sh ./_bin_example_mover_release.sh; break;;
        No ) break;;
    esac
done