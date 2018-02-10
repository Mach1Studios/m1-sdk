#!/bin/bash

#build all binaries and codesign them for use

echo "### Change Dir"
cd "/Volumes/git/m1-sdk/lib"

echo "### BUILD ANDROID ###"
echo "### BUILD NDK11 ###"
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-armeabi
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-armeabi-v7a
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-x86
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-x86-64
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-mips
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-arm64-v8a
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-arm64-v8a
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-mips64

echo "### BUILD NDK16b ###"
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-clang-libcxx  
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-clang-libcxx
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx

echo "### BUILD iOS ###"
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain ios-11-2-dep-9-0-device-bitcode-cxx11

echo "### BUILD macOS ###"
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain xcode

echo "### CODESIGN iOS & macOS ###"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/ios-11-2-dep-9-0-device-bitcode-cxx11/lib/libMach1DecodeCAPI.a"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1DecodeCAPI.dylib"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1DecodeCAPI.a"

echo "### Move and replace bins ###"}
echo "### Replacing Xcode/macOS ###"
yes | cp -rf _install/xcode ../binaries/
yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.dylib" "/Volumes/git/_BUILDENV_/of_v0.9.8_osx_release/addons/ofxMach1/libs/lib/osx/libMach1DecodeCAPI.dylib"
yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.dylib" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/lib/osx/libMach1DecodeCAPI.dylib"
yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.dylib" "../examples/mach1spatial-c/Unity/cs/Plugins/macOS/libMach1DecodeCAPI.bundle"
yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.dylib" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle"
yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.dylib" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodeCAPI.dylib"

echo "### Replacing iOS ###"
yes | cp -rf _install/ios-11-2-dep-9-0-device-bitcode-cxx11 ../binaries/
yes | cp -rf "_install/ios-11-2-dep-9-0-device-bitcode-cxx11/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/cs/Plugins/iOS/libMach1DecodeCAPI.a"
yes | cp -rf "_install/ios-11-2-dep-9-0-device-bitcode-cxx11/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodeCAPI.a"
yes | cp -rf "_install/ios-11-2-dep-9-0-device-bitcode-cxx11/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodeCAPI.a"

echo "### Replacing Android x86 & armaebi-v7a ###"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx" "../binaries/"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/android/AndroidNativeSound/app/src/main/jniLibs/armeabi-v7a/libMach1DecodeCAPI.so"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/cs/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so"
yes | cp -rf "_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx/lib/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodeCAPI.so"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx" "../binaries/"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/android/AndroidNativeSound/app/src/main/jniLibs/x86/libMach1DecodeCAPI.so"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/cs/Plugins/Android/x86/libMach1DecodeCAPI.so"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
yes | cp -rf "_install/android-ndk-r16b-api-21-x86-clang-libcxx/lib/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unreal Engine/Mach1DecodePlugin/Plugins/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodeCAPI.so"

echo "### NUMBERS OF SUCCESSFUL BUILDS:"
cd _logs/polly/
grep -rl "Installing:" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l

echo "### ERRORS CAUGHT:"
grep -rl "error" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
grep -rl "Error" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
grep -rl "failed" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
grep -rl "FAILED" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
