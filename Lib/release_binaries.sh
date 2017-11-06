#!/bin/bash

# Release branch prep script 
# moves all current built binaries into the binaries dir for pushing to Release/

mv _install/android-ndk-r11c-api-21-arm64-v8a/lib/libMach1DecodeCAPI.so ../binaries/android-ndk-r11c-api-21-arm64-v8a/lib/libMach1DecodeCAPI.so
mv _install/android-ndk-r11c-api-21-armeabi/lib/libMach1DecodeCAPI.so ../binaries/android-ndk-r11c-api-21-armeabi/lib/libMach1DecodeCAPI.so
mv _install/android-ndk-r11c-api-21-armeabi-v7a/lib/libMach1DecodeCAPI.so ../binaries/android-ndk-r11c-api-21-armeabi-v7a/lib/libMach1DecodeCAPI.so
mv _install/android-ndk-r11c-api-21-mips/lib/libMach1DecodeCAPI.so ../binaries/android-ndk-r11c-api-21-mips/lib/libMach1DecodeCAPI.so
mv _install/android-ndk-r11c-api-21-mips64/lib/libMach1DecodeCAPI.so ../binaries/android-ndk-r11c-api-21-mips64/lib/libMach1DecodeCAPI.so
mv _install/android-ndk-r11c-api-21-x86/lib/libMach1DecodeCAPI.so ../binaries/android-ndk-r11c-api-21-x86/lib/libMach1DecodeCAPI.so
mv _install/android-ndk-r11c-api-21-x86-64/lib/libMach1DecodeCAPI.so ../binaries/android-ndk-r11c-api-21-x86-64/lib/libMach1DecodeCAPI.so
mv _install/vs-14-2015/lib/libMach1DecodeCAPI.so ../binaries/vs-14-2015/lib/libMach1DecodeCAPI.so
mv _install/vs-14-2015-win64/lib/libMach1DecodeCAPI.so ../binaries/vs-14-2015-win64/lib/libMach1DecodeCAPI.so
mv _install/xcode/lib/libMach1DecodeCAPI.so ../binaries/xcode/lib/libMach1DecodeCAPI.so
mv _install/ios-11-0/lib/libMach1DecodeCAPI.so ../binaries/ios-11-0/lib/libMach1DecodeCAPI.so
