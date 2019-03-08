#!/bin/bash

#build all binaries and codesign them for use

echo "### Change Dir ###"
cd "/Volumes/git/m1-sdk/lib"

rm -rf _logs

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

echo "### BUILD RPI ###"
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain raspberrypi2-cxx11
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain raspberrypi3-cxx11

echo "### BUILD iOS ###"
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain ios
#/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain ios-11-4-dep-9-0-device-bitcode-cxx11

echo "### BUILD macOS ###"
/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain xcode

#echo "### BUILD cross-compile Linux ###"
#/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain linux-gcc-x64
#/Volumes/git/polly/bin/polly --clear --install --config Release --toolchain gcc-static-std


echo "### CODESIGN iOS & macOS ###"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/ios/lib/libMach1DecodeCAPI.a"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/ios/lib/libMach1DecodePositionalCAPI.a"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/ios/lib/libMach1EncodeCAPI.a"

codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1DecodeCAPI.a"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1DecodePositionalCAPI.a"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1EncodeCAPI.a"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1DecodeCAPI.bundle"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1DecodePositionalCAPI.bundle"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1EncodeCAPI.bundle"
