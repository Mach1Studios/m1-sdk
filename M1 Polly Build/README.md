# README #


### M1 SDK POLLY BUILD ###

To build M1 Library, install Polly (https://github.com/ruslo/polly).

## Recipes ##

* Mac OS build (xcode):

polly  --clear --install --config Release --toolchain xcode-gcc

* Android build (NDK r14):

set env var ANDROID_NDK_r11c to ndk path

polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-armeabi
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-armeabi-v7a
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-x86
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-x86-64
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-mips

* Windows (VS2015)

polly.bat --clear --install --config Release --toolchain vs-14-2015
polly.bat --clear --install --config Release --toolchain vs-14-2015-win64

* IOS

1. find the version of the installed iOS SDK:
xcodebuild -showsdks
2. then build with this version (for example 10.0):
polly --clear --install --config Release --toolchain ios-10-0
