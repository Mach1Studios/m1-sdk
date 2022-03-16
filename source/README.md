# README #


### Mach1Spatial libs build process ###

- To build libs, install Polly (https://github.com/ruslo/polly).
- Fill out the `m1-globallocal.mk` file and move it to `~/m1-globallocal.mk` for proper path variables
- run commands from `makefile` or run `make build-all` from this directory to test everything 

## Recipes ##

* Mac OS build (xcode) Example:

`polly  --clear --install --config Release --toolchain xcode`

* Android build (NDK r16b) Example:

set env var ANDROID_NDK_r16b to ndk path

`polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-clang-libcxx`
`polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx`
`polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-clang-libcxx`
`polly --clear --install --config Release --toolchain android-ndk-r16b-api-24-arm64-v8a-clang-libcxx14`
`polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx`

`polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-mips`
`polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-mips64`

* Windows (VS2015) Example

`polly.bat --clear --install --config Release --toolchain vs-14-2015`
`polly.bat --clear --install --config Release --toolchain vs-14-2015-win64`

* IOS

1. find the version of the installed iOS SDK:
`xcodebuild -showsdks`
2. to build full fat lib required for Pod, please build on xcode 8.3.1
`/Volumes/git/polly/bin/polly.py --clear --install --config Release --toolchain ios-10-3-dep-9-0-bitcode --ios-multiarch --ios-combined`
3. for travis-ci setup run `cd scripts && ./local-ios-setup.sh`

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
