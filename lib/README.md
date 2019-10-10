# README #


### Mach1Decode POLLY BUILD ###

To build Mach1Decode Library, install Polly (https://github.com/ruslo/polly).

## Recipes ##

* Mac OS build (xcode):

`polly  --clear --install --config Release --toolchain xcode`

* Android build (NDK r16b):

set env var ANDROID_NDK_r16b to ndk path

`polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-clang-libcxx`
`polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx`
`polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-clang-libcxx`
`polly --clear --install --config Release --toolchain android-ndk-r16b-api-24-arm64-v8a-clang-libcxx14`
`polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx`

`polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-mips`
`polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-mips64`

* Windows (VS2015)

`polly.bat --clear --install --config Release --toolchain vs-14-2015`
`polly.bat --clear --install --config Release --toolchain vs-14-2015-win64`

* IOS

1. find the version of the installed iOS SDK:
`xcodebuild -showsdks`
2. to build full fat lib required for Pod, please build on xcode 8.3.1
`/Volumes/git/polly/bin/polly.py --clear --install --config Release --toolchain ios-10-3-dep-9-0-bitcode --ios-multiarch --ios-combined`

If using iOS 11.0:
Add:
```set(IOS_SDK_VERSION 11.0)
set(IOS_DEPLOYMENT_SDK_VERSION 9.0)
set(POLLY_XCODE_COMPILER "clang")
polly_init(
    "iOS ${IOS_SDK_VERSION} Universal (iphoneos + iphonesimulator) / \
${POLLY_XCODE_COMPILER} / \
bitcode / \
c++14 support"
    "Xcode"
)
