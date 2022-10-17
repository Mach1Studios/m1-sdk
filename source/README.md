# README #


### Mach1Spatial libs build process ###

- Fill out the `m1-globallocal.mk` file and move it to `~/m1-globallocal.mk` for proper path variables
- run commands from `makefile` or run `make build-all` from this directory to test everything 

## Recipes ##

* Mac OS build (xcode) Example:

`cmake . -B_builds/xcode -GXcode -DCMAKE_INSTALL_PREFIX=`pwd`/_install/xcode`
`cmake --build _builds/xcode --target install`

* Android build (NDK r16b) Example:

set env var ANDROID_NDK_r16b to ndk path

`cmake . -B_builds/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx`
`cmake --build _builds/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx --config Release --target install`

`cmake . -B_builds/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx`
`cmake --build _builds/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx --config Release --target install`

`cmake . -B_builds/android-ndk-r16b-api-21-x86-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=x86 -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-x86-clang-libcxx`
`cmake --build _builds/android-ndk-r16b-api-21-x86-clang-libcxx --config Release --target install`

`cmake . -B_builds/android-ndk-r16b-api-21-x86-64-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=x86_64 -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-x86-64-clang-libcxx`
`cmake --build _builds/android-ndk-r16b-api-21-x86-64-clang-libcxx --config Release --target install`

* Windows (VS2017) Example

`cmake . -B_builds/windows-x86_64 -G "Visual Studio 15 2017" -A x64 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/windows-x86_64`
`cmake --build _builds/windows-x86_64 --target install`

* IOS

1. find the version of the installed iOS SDK:
`xcodebuild -showsdks`
2. to build full fat lib required for Pod, please build on xcode 8.3.1
`cmake . -B_builds/ios-11-3-dep-9-0-device-bitcode -GXcode -T buildsystem=1 -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=9.3 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/ios-11-3-dep-9-0-device-bitcode -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO -DCMAKE_IOS_INSTALL_COMBINED=YES`
`cmake --build _builds/ios-11-3-dep-9-0-device-bitcode --config Release --target install`

3. for travis-ci setup run `cd scripts && ./local-ios-setup.sh`

## TODO ##
- PR Merge for [../public](public) triggers a CD action/job for recompiling Mach1Transcode API/exec and upping the version