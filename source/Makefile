# Mach1 Spatial SDK

# local paths are in this file
include ~/m1-globallocal.mk

# getting OS type
ifeq ($(OS),Windows_NT)
	detected_OS := Windows
else
	detected_OS := $(shell uname)
endif

pull:
	git pull --recurse-submodules

setup:
	source $(EMSDK_PATH)/emsdk_env.sh

clear:
	rm -rf _install
	rm -rf _builds

test: clear
ifeq ($(detected_OS),Darwin)
	cd tests && ./_build_on_osx_for_osx.sh
else ifeq ($(detected_OS),Windows)
	cd tests && ./_build_on_win_for_win.bat
else
	cd tests && ./_build_on_linux_for_linux.sh
endif

test-linking:
ifeq ($(detected_OS),Darwin)
	mkdir -p builds
	g++ -o builds/spatial-decode-example ../examples/mach1spatial-c/commandline/spatial-decode-example/spatial-decode-example/main.cpp ../include/cpp/Mach1Decode.cpp -I../include/cpp -L_install/xcode/lib -lstdc++ -lpthread -lMach1DecodeCAPI
	g++ -o builds/spatial-encode-example ../examples/mach1spatial-c/commandline/spatial-encode-example/spatial-encode-example/main.cpp ../include/cpp/Mach1Encode.cpp -I../include/cpp -L_install/xcode/lib -lstdc++ -lpthread -lMach1EncodeCAPI
	g++ -o builds/spatial-transcode-example ../examples/mach1spatial-c/commandline/spatial-transcode-example/spatial-transcode-example/main.cpp ../include/cpp/Mach1Transcode.cpp -I../include/cpp -L_install/xcode/lib -lstdc++ -lMach1TranscodeCAPI
else ifeq ($(detected_OS),Windows)
	# windows testing
else
	mkdir -p builds
	g++ -o builds/spatial-decode-example ../examples/mach1spatial-c/commandline/spatial-decode-example/spatial-decode-example/main.cpp ../include/cpp/Mach1Decode.cpp -I../include/cpp -L_install/gcc/lib -lstdc++ -pthread -lMach1DecodeCAPI
	g++ -o builds/spatial-encode-example ../examples/mach1spatial-c/commandline/spatial-encode-example/spatial-encode-example/main.cpp ../include/cpp/Mach1Encode.cpp -I../include/cpp -L_install/gcc/lib -lstdc++ -pthread -lMach1EncodeCAPI
	g++ -o builds/spatial-transcode-example ../examples/mach1spatial-c/commandline/spatial-transcode-example/spatial-transcode-example/main.cpp ../include/cpp/Mach1Transcode.cpp -I../include/cpp -L_install/gcc/lib -lstdc++ -lMach1TranscodeCAPI
endif

build-all: clear test copy-public includes-dev includes-release build build-web build-android build-rpi build-cortexm0 deploy-dev deploy-release
	echo "REMINDER: Build FAT iOS lib first!"

build-ci-mac: clear test includes-dev includes-release build build-web deploy-dev deploy-release

build-ci-linux: clear test includes-dev includes-release build deploy-dev deploy-release

build-ci-win: clear test includes-dev includes-release build deploy-dev deploy-release

build: FORCE
ifeq ($(detected_OS),Darwin)
	mkdir -p "../mach1spatial-libs/xcode/lib"
	cmake . -B_builds/xcode -GXcode -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=ON -DBUILD_MACOS_BUNDLE=1 -DBUILD_SHARED_LIBS=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/xcode
	cmake --build _builds/xcode --config Release --target install
	rsync -c "_install/xcode/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/xcode/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/xcode/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/xcode/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a"
	#rsync -c "_install/xcode/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1Decode-minifiedCAPI.a"
	#echo "[NOTE] To compile for Universal Binary follow instructions below:"
	#cat  ./macOS-UB-readme.md
	#open _builds/xcode/Mach1SpatialLibraries.xcodeproj
else ifeq ($(detected_OS),Windows)
	cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -A Win32 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/vs-15-2017
	cmake --build _builds/vs-15-2017 --config Release --target install
	rsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Release/"
	rsync -c "_install/vs-15-2017/lib-shared" "../mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Release/"
	rsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/windows-x86"
	cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -A Win32 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/vs-15-2017
	cmake --build _builds/vs-15-2017 --config Debug --target installrsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Debug/"
	rsync -c "_install/vs-15-2017/lib-shared" "../mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Debug/"
	# MD x64
	cmake . -B_builds/vs-15-2017-win64 -G "Visual Studio 15 2017" -A x64 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/vs-15-2017-win64
	cmake --build _builds/vs-15-2017-win64 --config Release --target install
	rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Release/"
	rsync -c "_install/vs-15-2017-win64/lib-shared" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Release/"
	rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/windows-x86_64"
	cmake . -B_builds/vs-15-2017-win64 -G "Visual Studio 15 2017" -A x64 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/vs-15-2017-win64
	cmake --build _builds/vs-15-2017-win64 --config Debug --target install
	rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Debug/"
	rsync -c "_install/vs-15-2017-win64/lib-shared" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Debug/"
	# MT win32
	cmake . -B_builds/vs-15-2017-mt -G "Visual Studio 15 2017" -A Win32 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/vs-15-2017-mt
	cmake --build _builds/vs-15-2017-mt --config Release --target install
	rsync -c "_install/vs-15-2017-mt/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Release/"
	cmake . -B_builds/vs-15-2017-mt -G "Visual Studio 15 2017" -A Win32 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/vs-15-2017-mt
	cmake --build _builds/vs-15-2017-mt --config Debug --target install
	rsync -c "_install/vs-15-2017-mt/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Debug/"
	# MT x64
	cmake . -B_builds/vs-15-2017-win64-mt -G "Visual Studio 15 2017" -A x64 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/vs-15-2017-win64-mt
	cmake --build _builds/vs-15-2017-win64-mt --config Release --target install
	rsync -c "_install/vs-15-2017-win64-mt/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Release/"
	cmake . -B_builds/vs-15-2017-win64-mt -G "Visual Studio 15 2017" -A x64 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/vs-15-2017-win64-mt
	cmake --build _builds/vs-15-2017-win64-mt --config Debug --target install
	rsync -c "_install/vs-15-2017-win64-mt/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Debug/"
	cmake . -B_builds/gcc -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc
	cmake --build _builds/gcc --config Release --target install
endif

debug: FORCE
ifeq ($(detected_OS),Darwin)
	mkdir -p "../mach1spatial-libs/xcode/lib"
	cmake . -B_builds/xcode -GXcode -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=ON -DBUILD_MACOS_BUNDLE=1 -DBUILD_SHARED_LIBS=1 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/xcode
	cmake --build _builds/xcode --config Debug --target install
	rsync -c "_install/xcode/lib/libMach1DecodeCAPId.a" "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/xcode/lib/libMach1EncodeCAPId.a" "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/xcode/lib/libMach1TranscodeCAPId.a" "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/xcode/lib/libMach1DecodePositionalCAPId.a" "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a"
	#rsync -c "_install/xcode/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1Decode-minifiedCAPI.a"
else ifeq ($(detected_OS),Windows)
	cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -A Win32 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/vs-15-2017
	cmake --build _builds/vs-15-2017 --config Debug --target installrsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Debug/"
	cmake . -B_builds/vs-15-2017-win64 -G "Visual Studio 15 2017" -A x64 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/vs-15-2017-win64
	cmake --build _builds/vs-15-2017-win64 --config Debug --target install
else
	cmake . -B_builds/gcc -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc
	cmake --build _builds/gcc --config Debug --target install
endif

copy-public:
	rsync -c ../public/* $(M1SDK_RELEASE_PATH)

includes-dev: 
	rsync -c include/cpp/* ../mach1spatial-libs/arm-chipsets/beaglebone/include
	rsync -c include/cpp/* ../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/include
	rsync -c include/cpp/* ../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/include
	rsync -c include/cpp/* ../mach1spatial-libs/arm-chipsets/linux-arm/include
	rsync -c include/cpp/* ../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/include
	rsync -c include/cpp/* ../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/include
	rsync -c include/cpp/* ../mach1spatial-libs/linux/include
	rsync -c include/cpp/* ../mach1spatial-libs/xcode/include
	rsync -c include/cpp/* ../mach1spatial-libs/osx-10-13/include
	rsync -c include/cpp/* ../mach1spatial-libs/vs-15-2017-x86/include
	rsync -c include/cpp/* ../mach1spatial-libs/vs-15-2017-x86_64/include
	rsync -c include/cpp/* ../mach1spatial-libs/vs-16-2019-x86/include
	rsync -c include/cpp/* ../mach1spatial-libs/vs-16-2019-x86_64/include
	rsync -c include/cpp/* ../mach1spatial-libs/windows-x86/include
	rsync -c include/cpp/* ../mach1spatial-libs/windows-x86_64/include
	rsync -c include/java/* ../mach1spatial-libs/android-arm64-v8a/include
	rsync -c include/java/* ../mach1spatial-libs/android-armeabi-v7a/include
	rsync -c include/java/* ../mach1spatial-libs/android-x86/include
	rsync -c include/java/* ../mach1spatial-libs/android-x86_64/include
	rsync -c include/js/* ../mach1spatial-libs/web
	rsync -c include/swift/* ../mach1spatial-libs/ios/include

includes-release: 
	#rsync -c include/c#/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/c#
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/beaglebone/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/linux-arm/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/linux/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/xcode/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/osx-10-13/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86_64/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86_64/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/windows-x86/include
	rsync -c include/cpp/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/windows-x86_64/include
	rsync -c include/java/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-arm64-v8a/include
	rsync -c include/java/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-armeabi-v7a/include
	rsync -c include/java/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-x86/include
	rsync -c include/java/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-x86_64/include
	rsync -c include/js/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/web
	rsync -c include/swift/* $(M1SDK_RELEASE_PATH)/mach1spatial-libs/ios/include

build-mac-10-13:
	mkdir -p "../mach1spatial-libs/osx-10-13/lib"
	cmake . -B_builds/osx-10-13 -GXcode -DCMAKE_OSX_DEPLOYMENT_TARGET="10.13" -DCMAKE_INSTALL_PREFIX=`pwd`/_install/osx-10-13
	cmake --build _builds/osx-10-13 --config Release --target install
	rsync -c "_install/osx-10-13/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/osx-10-13/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/osx-10-13/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/osx-10-13/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1DecodePositionalCAPI.a"

build-ios: FORCE
	cmake . -B_builds/ios-11-3-dep-9-0-device-bitcode -GXcode -T buildsystem=1 -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=9.3 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/ios-11-3-dep-9-0-device-bitcode -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO -DCMAKE_IOS_INSTALL_COMBINED=YES
	cmake --build _builds/ios-11-3-dep-9-0-device-bitcode --config Release --target install
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1DecodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1DecodeCAPI.framework"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1EncodeCAPI.framework" "../mach1spatial-libs/ios/FrameworksFrameworks/libMach1EncodeCAPI.framework"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1DecodePositionalCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1DecodePositionalCAPI.framework"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1TranscodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1TranscodeCAPI.framework"
	rsync -c include/swift/* ../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes

build-ios-dev: FORCE
	cmake . -B_builds/ios-11-3-dep-9-0-device-bitcode -GXcode -T buildsystem=1 -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=9.3 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/ios-11-3-dep-9-0-device-bitcode -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO -DCMAKE_IOS_INSTALL_COMBINED=YES
	cmake --build _builds/ios-11-3-dep-9-0-device-bitcode --config Debug --target install
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1DecodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1DecodeCAPI.framework"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1EncodeCAPI.framework" "../mach1spatial-libs/ios/FrameworksFrameworks/libMach1EncodeCAPI.framework"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1DecodePositionalCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1DecodePositionalCAPI.framework"
	rsync -c "_install/ios-11-3-dep-9-0-device-bitcode/Frameworks/libMach1TranscodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1TranscodeCAPI.framework"
	rsync -c include/swift/* ../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes

build-rpi:
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain raspberrypi2-cxx11
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain raspberrypi3-cxx11
	rsync -c "_install/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/raspberrypi2-cxx11/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1Decode-minifiedCAPI.a"
	rsync -c "_install/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/raspberrypi3-cxx11/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1Decode-minifiedCAPI.a"

build-beaglebone:
	echo "[NOTE] Ensure to change polly toolchain to use c++14 instead of default c++11"
ifeq ($(detected_OS),Linux)
	cmake . -B_builds/gcc -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc
	cmake --build _builds/gcc --config Release --target install
	rsync -c "_install/gcc/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/linux/lib/libMach1DecodePositionalCAPI.a"
else
	cmake . -B_builds/linux-gcc-armhf -DCMAKE_TOOLCHAIN_FILE=./cmake/armhf-toolchain.cmake -DCMAKE_INSTALL_PREFIX=`pwd`/_install/linux-gcc-armhf
	cmake --build _builds/linux-gcc-armhf --config Release --target install
	rsync -c "_install/linux-gcc-armhf/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/linux-gcc-armhf/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/linux-gcc-armhf/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/linux-gcc-armhf/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/linux-gcc-armhf/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1Decode-minifiedCAPI.a"
endif

build-cortexm0:
	cmake . -B_builds/gcc-arm-cortexm0-cxx14 -DCMAKE_TOOLCHAIN_FILE=./cmake/cortex-m0-toolchain.cmake -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc-arm-cortexm0-cxx14 -DCXX_VERSION=14
	cmake --build _builds/gcc-arm-cortexm0-cxx14 --config Release --target install
	rsync -c "_install/gcc-arm-cortexm0-cxx14/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/gcc-arm-cortexm0-cxx14/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/gcc-arm-cortexm0-cxx14/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/gcc-arm-cortexm0-cxx14/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/gcc-arm-cortexm0-cxx14/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1Decode-minifiedCAPI.a"

build-armhfneonvfpv4:
	cmake . -B_builds/gcc-armhf-neon-vfpv4 -DCMAKE_TOOLCHAIN_FILE=./cmake/armhf-neon-vfpv4-toolchain.cmake -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc-armhf-neon-vfpv4
	cmake --build _builds/gcc-armhf-neon-vfpv4 --config Release --target install
	rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/gcc-armhf-neon-vfpv4/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1Decode-minifiedCAPI.a"

build-web:
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1DecodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1DecodeEmscripten.js -o include/js/Mach1Decode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1DecodePositionalModule'" -s ALLOW_TABLE_GROWTH=1 -Ideps/ --pre-js Mach1DecodePositionalEmscripten.js -o include/js/Mach1DecodePositional.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodePositionalCore.cpp Mach1DecodePositionalCAPI.cpp Mach1DecodePositionalEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1EncodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1EncodeEmscripten.js -o include/js/Mach1Encode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1TranscodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1TranscodeEmscripten.js -o include/js/Mach1Transcode.js -Iinclude -I../public Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1TranscodeCore.cpp Mach1TranscodeCAPI.cpp Mach1TranscodeEmscripten.cpp Mach1GenerateCoeffs.cpp deps/M1DSP/M1DSPUtilities.cpp deps/M1DSP/M1DSPFilters.cpp
	# COPYING FOR WASM
	rsync -c include/js/* ../mach1spatial-libs/web
	# NODEJS
	rsync -c "include/js/Mach1Decode.js" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1Decode.js"
	rsync -c "include/js/Mach1Decode.wasm" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1Decode.wasm"
	rsync -c "include/js/Mach1DecodePositional.js" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1DecodePositional.js"
	rsync -c "include/js/Mach1DecodePositional.wasm" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1DecodePositional.wasm"
	rsync -c "include/js/Mach1Encode.js" "../examples/mach1spatial-nodejs/mach1spatial-encode/lib/Mach1Encode.js"
	rsync -c "include/js/Mach1Encode.wasm" "../examples/mach1spatial-nodejs/mach1spatial-encode/lib/Mach1Encode.wasm"
	rsync -c "include/js/Mach1Transcode.js" "../examples/mach1spatial-nodejs/mach1spatial-transcode/lib/Mach1Transcode.js"
	rsync -c "include/js/Mach1Transcode.wasm" "../examples/mach1spatial-nodejs/mach1spatial-transcode/lib/Mach1Transcode.wasm"
	# EXAMPLES
	rsync -c include/js/* ../examples/mach1spatial-web/js
	rsync -c "include/js/Mach1Decode.js" "../examples/mach1spatial-web/m1-web-spatialaudioplayer/js/Mach1Decode.js"
	rsync -c "include/js/Mach1Decode.wasm" "../examples/mach1spatial-web/m1-web-spatialaudioplayer/js/Mach1Decode.wasm"
	# OFXMACH1
	rsync -c ../mach1spatial-libs/web/* ../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/emscripten/

build-android:
	cmake . -B_builds/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx
	cmake --build _builds/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx --config Release --target install
	cmake . -B_builds/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
	cmake --build _builds/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx --config Release --target install
	cmake . -B_builds/android-ndk-r16b-api-21-x86-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=x86 -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-x86-clang-libcxx
	cmake --build _builds/android-ndk-r16b-api-21-x86-clang-libcxx --config Release --target install
	cmake . -B_builds/android-ndk-r16b-api-21-x86-64-clang-libcxx -DCMAKE_SYSTEM_NAME=Android -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_ARCH_ABI=x86_64 -DCMAKE_ANDROID_NDK=${ANDROID_NDK_r16b} -DCMAKE_ANDROID_STL_TYPE=gnustl_static -DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-ndk-r16b-api-21-x86-64-clang-libcxx
	cmake --build _builds/android-ndk-r16b-api-21-x86-64-clang-libcxx --config Release --target install
	mv _install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx _install/android-armeabi-v7a
	mv _install/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx _install/android-arm64-v8a
	mv _install/android-ndk-r16b-api-21-x86-clang-libcxx _install/android-x86
	mv _install/android-ndk-r16b-api-21-x86-64-clang-libcxx _install/android-x86_64
	# INSTALL ANDROID LIBS
	rsync -c "_install/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/android-arm64-v8a/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/android-arm64-v8a/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/android-arm64-v8a/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/android-x86/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/android-x86/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/android-x86/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/android-x86/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a"
	rsync -c "_install/android-x86_64/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/android-x86_64/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/android-x86_64/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/android-x86_64/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1DecodePositionalCAPI.a"

deploy-windows:

build-linux: FORCE
	cmake . -B_builds/gcc -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc
	cmake --build _builds/gcc --config Release --target install

deploy-linux: build-linux
	rsync -c "_install/gcc/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/linux/lib/libMach1DecodePositionalCAPI.a"

deploy-linux-arm: build-linux
	rsync -c "_install/gcc/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1DecodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1EncodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1TranscodeCAPI.a"
	rsync -c "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1DecodePositionalCAPI.a"

deploy-release: deploy-dev
	#replace all mach1spatial-libs in github release 
	echo "### Copying all successful bins to /mach1spatial-libs \###"
	rsync -c "../mach1spatial-libs/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/"
	echo "### Move and replace bins \###"
	echo "### Replacing Xcode/macOS \###"
	rsync -c "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodePositionalCAPI.a"
	echo "### Moving iOS \###"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodePositionalCAPI.a"
	echo "### Replacing Android \###"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodePositionalCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodePositionalCAPI.a"
	echo "### Copying for ofxMach1 \###"
	rsync -c "include/cpp/Mach1Decode.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.h"
	rsync -c "include/cpp/Mach1Decode.cpp" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.cpp"
	rsync -c "include/cpp/Mach1DecodeCAPI.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodeCAPI.h"
	rsync -c "include/cpp/Mach1DecodePositional.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.h"
	rsync -c "include/cpp/Mach1DecodePositional.cpp" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.cpp"
	rsync -c "include/cpp/Mach1DecodePositionalCAPI.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodePositionalCAPI.h"
	rsync -c "include/cpp/Mach1Encode.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.h"
	rsync -c "include/cpp/Mach1Encode.cpp" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.cpp"
	rsync -c "include/cpp/Mach1EncodeCAPI.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1EncodeCAPI.h"
	rsync -c "include/cpp/Mach1Transcode.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Transcode.h"
	rsync -c "include/cpp/Mach1Transcode.cpp" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Transcode.cpp"
	rsync -c "include/cpp/Mach1TranscodeCAPI.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1TranscodeCAPI.h"
	rsync -c "include/cpp/Mach1Point3D.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point3D.h"
	rsync -c "include/cpp/Mach1Point4D.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point4D.h"
	echo "### Copying Windows libs \###"
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Release
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Release
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Release
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Release
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Release
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Release
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86/lib/Static/MD/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86_64/lib/Static/MD/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86/lib/Static/MD/Release
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86_64/lib/Static/MD/Release
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86/lib/Static/MT/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86_64/lib/Static/MT/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86/lib/Static/MT/Release
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86_64/lib/Static/MT/Release
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86/lib/Dynamic/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86_64/lib/Dynamic/Debug
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86/lib/Dynamic/Release
	mkdir -p $(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86_64/lib/Dynamic/Release
	rsync -c "../mach1spatial-libs/vs-15-2017-x86" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86"
	rsync -c "../mach1spatial-libs/vs-15-2017-x86_64" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-15-2017-x86_64"
	rsync -c "../mach1spatial-libs/vs-16-2019-x86" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86"
	rsync -c "../mach1spatial-libs/vs-16-2019-x86_64" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/vs-16-2019-x86_64"
	rsync -c "../mach1spatial-libs/windows-x86" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/windows-x86"
	rsync -c "../mach1spatial-libs/windows-x86_64" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/windows-x86_64"
	echo "### DELETE all lib-shared dirs"
	cd $(M1SDK_RELEASE_PATH)/mach1spatial-libs && find . -type d -name 'lib-shared' -print0 | xargs -0 rm -rf
	cd $(M1SDK_RELEASE_PATH)/mach1spatial-libs && find . -type d -name 'libBundle' -print0 | xargs -0 rm -rf

deploy-dev:
	# general copy over
	echo "### Copying all successful bins to /mach1spatial-libs \###"
	rsync -c "_install/" "../mach1spatial-libs/"
	# copy over mac libs
	echo "### Move and replace bins \###"
	echo "### Replacing Xcode/macOS \###"
	rsync -c "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodePositionalCAPI.a"
	rsync -c "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodePositionalCAPI.a"
	echo "### Replacing iOS \###"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodePositionalCAPI.a"
	echo "### Moving iOS \###"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodePositionalCAPI.a"
	echo "### Copying to CocoaPods Repo \###"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodePositionalCAPI.a"
	rsync -c include/swift/* ../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes
	echo "### Replacing Android \###"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodePositionalCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodePositionalCAPI.a"

	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodePositionalCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodePositionalCAPI.a"
	# Jitpack
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1DecodePositionalCAPI.a"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1DecodePositionalCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1DecodePositionalCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86_64/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86_64/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86_64/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/android-x86_64/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1DecodePositionalCAPI.a"
	rsync -c include/cpp/*CAPI.h ../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include
	rsync -c include/cpp/*D.h ../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include
	rsync -c include/java/jni/cpp/mach1/* ../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1
	rsync -c include/java/jni/java/com/mach1/spatiallibs/* ../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/java/com/mach1/spatiallibs
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/mach1-decode-example/src/main/jniLibs/armeabi-v7a/libMach1DecodeCAPI.a"
	rsync -c include/cpp/Mach1Decode.cpp ../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include
	rsync -c include/cpp/Mach1Decode.h ../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include
	rsync -c include/cpp/Mach1DecodeCAPI.h ../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include
	rsync -c include/cpp/*D.h ../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/mach1-decode-example/src/main/cpp
	echo "### Copying for ofxMach1 \###"
	rsync -c "include/cpp/Mach1Decode.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.h"
	rsync -c "include/cpp/Mach1Decode.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.cpp"
	rsync -c "include/cpp/Mach1DecodeCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodeCAPI.h"
	rsync -c "include/cpp/Mach1DecodePositional.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.h"
	rsync -c "include/cpp/Mach1DecodePositional.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.cpp"
	rsync -c "include/cpp/Mach1DecodePositionalCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodePositionalCAPI.h"
	rsync -c "include/cpp/Mach1Encode.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.h"
	rsync -c "include/cpp/Mach1Encode.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.cpp"
	rsync -c "include/cpp/Mach1EncodeCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1EncodeCAPI.h"
	rsync -c "include/cpp/Mach1Transcode.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Transcode.h"
	rsync -c "include/cpp/Mach1Transcode.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Transcode.cpp"
	rsync -c "include/cpp/Mach1TranscodeCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1TranscodeCAPI.h"
	rsync -c "include/cpp/Mach1Point3D.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point3D.h"
	rsync -c "include/cpp/Mach1Point4D.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point4D.h"
	echo "### Copying for UE \###"
	rsync -c include/cpp/*CAPI.h ../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/include
	rsync -c include/cpp/*.h ../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/Source/Mach1DecodePlugin/Public
	rm -f ../examples/mach1spatial-c/Unreal\ Engine/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/Source/Mach1DecodePlugin/Public/*CAPI.h
	rsync -c include/cpp/*.cpp ../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/Source/Mach1DecodePlugin/Private
	echo "### Copying libs for Unity \###"
	rsync -c "../mach1spatial-libs/xcode/libBundle/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle"
	rsync -c "../mach1spatial-libs/xcode/libBundle/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1EncodeCAPI.bundle"
	rsync -c "../mach1spatial-libs/xcode/libBundle/libMach1TranscodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1TranscodeCAPI.bundle"
	rsync -c "../mach1spatial-libs/xcode/libBundle/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodePositionalCAPI.bundle"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodePositionalCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1EncodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1TranscodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodePositionalCAPI.so"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1EncodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1TranscodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodePositionalCAPI.so"
	rsync -c "../mach1spatial-libs/android-x86/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-x86/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1EncodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-x86/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1TranscodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-x86/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodePositionalCAPI.so"
	rsync -c "../mach1spatial-libs/xcode/libBundle/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle"
	rsync -c "../mach1spatial-libs/xcode/libBundle/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1EncodeCAPI.bundle"
	rsync -c "../mach1spatial-libs/xcode/libBundle/libMach1TranscodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1TranscodeCAPI.bundle"
	rsync -c "../mach1spatial-libs/xcode/libBundle/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1DecodePositionalCAPI.bundle"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1DecodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1EncodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1TranscodeCAPI.a"
	rsync -c "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1DecodePositionalCAPI.a"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1EncodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1TranscodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-armeabi-v7a/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodePositionalCAPI.so"
	rsync -c "../mach1spatial-libs/android-x86/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-x86/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1EncodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-x86/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1TranscodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-x86/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1DecodePositionalCAPI.so"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1EncodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1TranscodeCAPI.so"
	rsync -c "../mach1spatial-libs/android-arm64-v8a/lib-shared/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodePositionalCAPI.so"
	echo "### Copying ofxMach1 to dev local \###"
	rsync -c "../examples/mach1spatial-c/openframeworks/ofxMach1" "$(OF_PATH)/addons/"
	echo "### DELETE all lib-shared dirs \###"
	cd ../mach1spatial-libs && find . -type d -name 'lib-shared' -print0 | xargs -0 rm -rf
	cd ../mach1spatial-libs && find . -type d -name 'libBundle' -print0 | xargs -0 rm -rf

# place anything you need all commands to run here
FORCE:
