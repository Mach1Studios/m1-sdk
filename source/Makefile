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
	cd tests && $(POLLY_BIN_PATH) --clear --install --config Release --toolchain xcode
	./tests/_install/xcode/bin/Mach1EncodeTest
	./tests/_install/xcode/bin/Mach1DecodeTest
	./tests/_install/xcode/bin/Mach1DecodePositionalTest
	./tests/_install/xcode/bin/Mach1SpatialTests
	./tests/_install/xcode/bin/Mach1TranscodeTests
else ifeq ($(detected_OS),Windows)
	cd tests && python $(POLLY_PY_PATH) --clear --install --config Release --toolchain vs-15-2017-win64
else
	cd tests && $(POLLY_BIN_PATH) --clear --install --config Release --toolchain gcc
	./tests/_install/gcc/bin/Mach1EncodeTest
	./tests/_install/gcc/bin/Mach1DecodeTest
	./tests/_install/gcc/bin/Mach1DecodePositionalTest
	./tests/_install/gcc/bin/Mach1SpatialTests
	./tests/_install/gcc/bin/Mach1TranscodeTests
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
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain xcode
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/lib/libMach1DecodeCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/lib/libMach1DecodePositionalCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/lib/libMach1EncodeCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/libBundle/libMach1DecodeCAPI.bundle"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/libBundle/libMach1DecodePositionalCAPI.bundle"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/libBundle/libMach1EncodeCAPI.bundle"
else ifeq ($(detected_OS),Windows)
	python $(POLLY_PY_PATH) --clear --install --config Release --toolchain vs-15-2017
	python $(POLLY_PY_PATH) --clear --install --config Release --toolchain vs-15-2017-win64
else
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain gcc
endif

copy-public:
	yes | cp -rf ../public $(M1SDK_RELEASE_PATH)/

includes-dev: 
	#yes | cp -rf "external/c#" "../mach1spatial-libs/c#"
	yes | cp -rf "external/cpp/" "../mach1spatial-libs/arm-chipsets/beaglebone/include"
	yes | cp -rf "external/cpp/" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/include"
	yes | cp -rf "external/cpp/" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/include"
	yes | cp -rf "external/cpp/" "../mach1spatial-libs/arm-chipsets/linux-arm/include"
	yes | cp -rf "external/cpp/" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/include"
	yes | cp -rf "external/cpp/" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/include"
	yes | cp -rf "external/cpp/" "../mach1spatial-libs/linux/include"
	yes | cp -rf "external/cpp/" "../mach1spatial-libs/xcode/include"
	yes | cp -rf "external/java/" "../mach1spatial-libs/android-arm64-v8a/include"
	yes | cp -rf "external/java/" "../mach1spatial-libs/android-armeabi-v7a/include"
	yes | cp -rf "external/java/" "../mach1spatial-libs/android-x86/include"
	yes | cp -rf "external/java/" "../mach1spatial-libs/android-x86_64/include"
	yes | cp -rf "external/js/" "../mach1spatial-libs/web"
	yes | cp -rf "external/swift/" "../mach1spatial-libs/ios/include"

includes-release: 
	#yes | cp -rf "external/c#" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/c#"
	yes | cp -rf "external/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/beaglebone/include"
	yes | cp -rf "external/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/include"
	yes | cp -rf "external/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/include"
	yes | cp -rf "external/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/linux-arm/include"
	yes | cp -rf "external/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/include"
	yes | cp -rf "external/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/include"
	yes | cp -rf "external/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/linux/include"
	yes | cp -rf "external/cpp/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/xcode/include"
	yes | cp -rf "external/java/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-arm64-v8a/include"
	yes | cp -rf "external/java/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-armeabi-v7a/include"
	yes | cp -rf "external/java/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-x86/include"
	yes | cp -rf "external/java/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/android-x86_64/include"
	yes | cp -rf "external/js/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/web"
	yes | cp -rf "external/swift/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/ios/include"

build-mac:
	mkdir -p "../mach1spatial-libs/xcode/lib"
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain xcode
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/lib/libMach1DecodeCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/lib/libMach1DecodePositionalCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/lib/libMach1TranscodeCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/lib/libMach1EncodeCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/libBundle/libMach1DecodeCAPI.bundle"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/libBundle/libMach1DecodePositionalCAPI.bundle"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/libBundle/libMach1TranscodeCAPI.bundle"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/xcode/libBundle/libMach1EncodeCAPI.bundle"
	yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/xcode/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/xcode/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/xcode/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a"
	#yes | cp -rf "_install/xcode/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/xcode/lib/libMach1Decode-minifiedCAPI.a"
	#echo "[NOTE] To compile for Universal Binary follow instructions below:"
	#cat  ./macOS-UB-readme.md
	#open _builds/xcode/Mach1SpatialLibraries.xcodeproj

build-mac-10-13:
	mkdir -p "../mach1spatial-libs/osx-10-13/lib"
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain osx-10-13
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/osx-10-13/lib/libMach1DecodeCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/osx-10-13/lib/libMach1DecodePositionalCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/osx-10-13/lib/libMach1TranscodeCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "$(M1SDK_PATH)/source/_install/osx-10-13/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/osx-10-13/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/osx-10-13/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/osx-10-13/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/osx-10-13/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1DecodePositionalCAPI.a"	

build-ios: FORCE
	mkdir -p _builds/ios-10-3-dep-9-0-bitcode/Release-iphoneos # adding this for some weird bug on older macos
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain ios-10-3-dep-9-0-bitcode --ios-multiarch --ios-combined
	yes | cp -rf "_install/ios-10-3-dep-9-0-bitcode/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/ios-10-3-dep-9-0-bitcode/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/ios-10-3-dep-9-0-bitcode/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/ios-10-3-dep-9-0-bitcode/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/ios-10-3-dep-9-0-bitcode/Frameworks/libMach1DecodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1DecodeCAPI.framework"
	yes | cp -rf "_install/ios-10-3-dep-9-0-bitcode/Frameworks/libMach1EncodeCAPI.framework" "../mach1spatial-libs/ios/FrameworksFrameworks/libMach1EncodeCAPI.framework"
	yes | cp -rf "_install/ios-10-3-dep-9-0-bitcode/Frameworks/libMach1DecodePositionalCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1DecodePositionalCAPI.framework"
	yes | cp -rf "_install/ios-10-3-dep-9-0-bitcode/Frameworks/libMach1TranscodeCAPI.framework" "../mach1spatial-libs/ios/Frameworks/libMach1TranscodeCAPI.framework"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodePositionalCAPI.a"
	yes | cp -rf external/swift/* "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes/"

build-ios-dev: FORCE
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain ios --ios-multiarch --ios-combined
	yes | cp -rf "_install/ios/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/ios/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/ios/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/ios/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodePositionalCAPI.a"
	yes | cp -rf external/swift/* "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes/"

build-rpi:
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain raspberrypi2-cxx11
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain raspberrypi3-cxx11
	yes | cp -rf "_install/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/raspberrypi2-cxx11/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1Decode-minifiedCAPI.a"
	yes | cp -rf "_install/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/raspberrypi3-cxx11/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1Decode-minifiedCAPI.a"

build-beaglebone:
	echo "[NOTE] Ensure to change polly toolchain to use c++14 instead of default c++11"
ifeq ($(detected_OS),Linux)
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain gcc
	yes | cp -rf "_install/gcc/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1Decode-minifiedCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1DecodeCAPI.a" "/usr/local/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1EncodeCAPI.a" "/usr/local/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1TranscodeCAPI.a" "/usr/local/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "/usr/local/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1Decode-minifiedCAPI.a" "/usr/local/lib/libMach1Decode-minifiedCAPI.a"
else
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain linux-gcc-armhf
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/beaglebone/lib/libMach1Decode-minifiedCAPI.a"
endif

build-cortexm0:
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain gcc-arm-cortexm0-cxx14
	yes | cp -rf "_install/gcc-arm-cortexm0-cxx14/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/gcc-arm-cortexm0-cxx14/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/gcc-arm-cortexm0-cxx14/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/gcc-arm-cortexm0-cxx14/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/gcc-arm-cortexm0-cxx14/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-arm-cortexm0-cxx14/lib/libMach1Decode-minifiedCAPI.a"

build-armhfneonvfpv4:
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain gcc-armhf-neon-vfpv4
	yes | cp -rf "_install/gcc-armhf-neon-vfpv4/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/gcc-armhf-neon-vfpv4/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/gcc-armhf-neon-vfpv4/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/gcc-armhf-neon-vfpv4/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/gcc-armhf-neon-vfpv4/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/gcc-armhf-neon-vfpv4/lib/libMach1Decode-minifiedCAPI.a"

build-web:
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1DecodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1DecodeEmscripten.js -o external/js/Mach1Decode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1DecodePositionalModule'" -s ALLOW_TABLE_GROWTH=1 -Iinclude/ --pre-js Mach1DecodePositionalEmscripten.js -o external/js/Mach1DecodePositional.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodePositionalCore.cpp Mach1DecodePositionalCAPI.cpp Mach1DecodePositionalEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1EncodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1EncodeEmscripten.js -o external/js/Mach1Encode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1TranscodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1TranscodeEmscripten.js -o external/js/Mach1Transcode.js -Iinclude -I../public Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1TranscodeCore.cpp Mach1TranscodeCAPI.cpp Mach1TranscodeEmscripten.cpp Mach1GenerateCoeffs.cpp include/M1DSP/M1DSPUtilities.cpp include/M1DSP/M1DSPFilters.cpp
	yes | cp -rf "external/js/Mach1Decode.js" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1Decode.js"
	yes | cp -rf "external/js/Mach1Decode.wasm" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1Decode.wasm"
	yes | cp -rf "external/js/Mach1DecodePositional.js" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1DecodePositional.js"
	yes | cp -rf "external/js/Mach1DecodePositional.wasm" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1DecodePositional.wasm"
	yes | cp -rf "external/js/Mach1Encode.js" "../examples/mach1spatial-nodejs/mach1spatial-encode/lib/Mach1Encode.js"
	yes | cp -rf "external/js/Mach1Encode.wasm" "../examples/mach1spatial-nodejs/mach1spatial-encode/lib/Mach1Encode.wasm"
	yes | cp -rf "external/js/Mach1Transcode.js" "../examples/mach1spatial-nodejs/mach1spatial-transcode/lib/Mach1Transcode.js"
	yes | cp -rf "external/js/Mach1Transcode.wasm" "../examples/mach1spatial-nodejs/mach1spatial-transcode/lib/Mach1Transcode.wasm"
	yes | cp -rf "external/js/Mach1Decode.js" "../examples/mach1spatial-web/js/Mach1Decode.js"
	yes | cp -rf "external/js/Mach1Decode.wasm" "../examples/mach1spatial-web/js/Mach1Decode.wasm"
	yes | cp -rf "external/js/Mach1DecodePositional.js" "../examples/mach1spatial-web/js/Mach1DecodePositional.js"
	yes | cp -rf "external/js/Mach1DecodePositional.wasm" "../examples/mach1spatial-web/js/Mach1DecodePositional.wasm"
	yes | cp -rf "external/js/Mach1Encode.js" "../examples/mach1spatial-web/js/Mach1Encode.js"
	yes | cp -rf "external/js/Mach1Encode.wasm" "../examples/mach1spatial-web/js/Mach1Encode.wasm"
	yes | cp -rf "external/js/Mach1Transcode.js" "../examples/mach1spatial-web/js/Mach1Transcode.js"
	yes | cp -rf "external/js/Mach1Transcode.wasm" "../examples/mach1spatial-web/js/Mach1Transcode.wasm"
	yes | cp -rf "external/js/Mach1Decode.js" "../examples/mach1spatial-web/m1-web-spatialaudioplayer/js/Mach1Decode.js"
	yes | cp -rf "external/js/Mach1Decode.wasm" "../examples/mach1spatial-web/m1-web-spatialaudioplayer/js/Mach1Decode.wasm"

build-android:
	python $(POLLY_PY_PATH) --clear --install --config Release --toolchain android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx
	python $(POLLY_PY_PATH) --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
	python $(POLLY_PY_PATH) --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-clang-libcxx
	python $(POLLY_PY_PATH) --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-64-clang-libcxx
ifeq ($(detected_OS),Darwin)
	yes | mv _install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx _install/android-armeabi-v7a
	yes | mv _install/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx _install/android-arm64-v8a
	yes | mv _install/android-ndk-r16b-api-21-x86-clang-libcxx _install/android-x86
	yes | mv _install/android-ndk-r16b-api-21-x86-64-clang-libcxx _install/android-x86_64
	yes | cp -rf "_install/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" 
	yes | cp -rf "_install/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/android-arm64-v8a/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/android-arm64-v8a/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/android-arm64-v8a/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/android-x86/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/android-x86/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/android-x86/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/android-x86/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/android-x86_64/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/android-x86_64/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/android-x86_64/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/android-x86_64/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/android-x86_64/lib/libMach1DecodePositionalCAPI.a"
	# copy for Unity
	yes | cp -rf "_install/android-armeabi-v7a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so" 
	yes | cp -rf "_install/android-armeabi-v7a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1EncodeCAPI.so"
	yes | cp -rf "_install/android-armeabi-v7a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1TranscodeCAPI.so"
	yes | cp -rf "_install/android-armeabi-v7a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "_install/android-arm64-v8a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodeCAPI.so"
	yes | cp -rf "_install/android-arm64-v8a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1EncodeCAPI.so"
	yes | cp -rf "_install/android-arm64-v8a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1TranscodeCAPI.so"
	yes | cp -rf "_install/android-arm64-v8a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "_install/android-x86/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
	yes | cp -rf "_install/android-x86/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1EncodeCAPI.so"
	yes | cp -rf "_install/android-x86/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1TranscodeCAPI.so"
	yes | cp -rf "_install/android-x86/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "_install/android-x86_64/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86_64/libMach1DecodeCAPI.so"
	yes | cp -rf "_install/android-x86_64/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86_64/libMach1EncodeCAPI.so"
	yes | cp -rf "_install/android-x86_64/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86_64/libMach1TranscodeCAPI.so"
	yes | cp -rf "_install/android-x86_64/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86_64/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "_install/android-armeabi-v7a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so" 
	yes | cp -rf "_install/android-armeabi-v7a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1EncodeCAPI.so"
	yes | cp -rf "_install/android-armeabi-v7a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1TranscodeCAPI.so"
	yes | cp -rf "_install/android-armeabi-v7a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "_install/android-arm64-v8a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodeCAPI.so"
	yes | cp -rf "_install/android-arm64-v8a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1EncodeCAPI.so"
	yes | cp -rf "_install/android-arm64-v8a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1TranscodeCAPI.so"
	yes | cp -rf "_install/android-arm64-v8a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "_install/android-x86/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
	yes | cp -rf "_install/android-x86/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1EncodeCAPI.so"
	yes | cp -rf "_install/android-x86/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1TranscodeCAPI.so"
	yes | cp -rf "_install/android-x86/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "_install/android-x86_64/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86_64/libMach1DecodeCAPI.so"
	yes | cp -rf "_install/android-x86_64/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86_64/libMach1EncodeCAPI.so"
	yes | cp -rf "_install/android-x86_64/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86_64/libMach1TranscodeCAPI.so"
	yes | cp -rf "_install/android-x86_64/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86_64/libMach1DecodePositionalCAPI.so"
else ifeq ($(detected_OS),Windows)
	# copy over android from pull
	xcopy _install\android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx\lib ..\mach1spatial-libs\android-arm64-v8a\lib /Y
	xcopy _install\android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx\lib ..\mach1spatial-libs\android-armeabi-v7a\lib /Y
	xcopy _install\android-ndk-r16b-api-21-x86-clang-libcxx\lib ..\mach1spatial-libs\android-x86\lib /Y
	xcopy _install\android-ndk-r16b-api-21-x86-64-clang-libcxx\lib ..\mach1spatial-libs\android-x86_64\lib /Y
	xcopy _install\android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx\libUnity ..\mach1spatial-libs\android-arm64-v8a\libUnity /Y
	xcopy _install\android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx\libUnity ..\mach1spatial-libs\android-armeabi-v7a\libUnity /Y
	xcopy _install\android-ndk-r16b-api-21-x86-clang-libcxx\libUnity ..\mach1spatial-libs\android-x86\libUnity /Y
	xcopy _install\android-ndk-r16b-api-21-x86-64-clang-libcxx\libUnity ..\mach1spatial-libs\android-x86_64\libUnity /Y
	del /f /s /q _android 1>nul
	rmdir /s /q _android
	mkdir ..\mach1spatial-libs\android-arm64-v8a
	mkdir ..\mach1spatial-libs\android-armeabi-v7a
	mkdir ..\mach1spatial-libs\android-x86
	mkdir ..\mach1spatial-libs\android-x86_64
endif
	# cleanup
	rm -rf _install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
	rm -rf _install/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx
	rm -rf _install/android-ndk-r16b-api-21-x86-clang-libcxx
	rm -rf _install/android-ndk-r16b-api-21-x86-64-clang-libcxx

deploy-windows:
	# copy over windows libs
	xcopy _install\vs-15-2017\lib ..\mach1spatial-libs\windows-x86 /Y
	xcopy _install\vs-15-2017-win64\lib ..\mach1spatial-libs\windows-x86_64 /Y
	xcopy _install\vs-15-2017\libUnity ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1UnityDecodeTest\Assets\Mach1\Plugins\Windows\x86 /Y
	xcopy _install\vs-15-2017-win64\libUnity ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1UnityDecodeTest\Assets\Mach1\Plugins\Windows\x86_64 /Y
	xcopy _install\vs-15-2017\libUnity ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1DSPGraph\Assets\Mach1\Plugins\Windows\x86 /Y
	xcopy _install\vs-15-2017-win64\libUnity ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1DSPGraph\Assets\Mach1\Plugins\Windows\x86_64 /Y
	xcopy _install\vs-15-2017\lib "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win32" /Y
	xcopy _install\vs-15-2017\libUnity "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win32" /Y
	xcopy _install\vs-15-2017-win64\lib "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win64" /Y
	xcopy _install\vs-15-2017-win64\libUnity "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win64" /Y
	xcopy _install\vs-15-2017\lib ..\examples\mach1spatial-c\openframeworks\ofxMach1\libs\libmach1\lib\vs\x86 /Y
	xcopy _install\vs-15-2017-win64\lib ..\examples\mach1spatial-c\openframeworks\ofxMach1\libs\libmach1\lib\vs\x64 /Y

test-linux: FORCE
	cd tests && sh ./_build_on_linux_for_linux.sh

build-linux: FORCE
	$(POLLY_BIN_PATH) --clear --install --config Release --toolchain gcc

deploy-linux: build-linux
	yes | cp -rf "_install/gcc/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/linux/lib/libMach1DecodePositionalCAPI.a"

deploy-linux-arm: build-linux
	yes | cp -rf "_install/gcc/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/linux-arm/lib/libMach1DecodePositionalCAPI.a"

deploy-release: deploy-dev
	#replace all mach1spatial-libs in github release 
	echo "### Copying all successful bins to /mach1spatial-libs \###"
	yes | cp -rf "../mach1spatial-libs/" "$(M1SDK_RELEASE_PATH)/mach1spatial-libs/"
	echo "### Move and replace bins \###"
	echo "### Replacing Xcode/macOS \###"
	yes | cp -rf "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodePositionalCAPI.a"
	echo "### Moving iOS \###"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodePositionalCAPI.a"
	echo "### Replacing Android \###"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodePositionalCAPI.a"
	echo "### Copying for ofxMach1 \###"
	yes | cp -rf "external/cpp/Mach1Decode.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.h"
	yes | cp -rf "external/cpp/Mach1Decode.cpp" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.cpp"
	yes | cp -rf "external/cpp/Mach1DecodeCAPI.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodeCAPI.h"
	yes | cp -rf "external/cpp/Mach1DecodePositional.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.h"
	yes | cp -rf "external/cpp/Mach1DecodePositional.cpp" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.cpp"
	yes | cp -rf "external/cpp/Mach1DecodePositionalCAPI.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodePositionalCAPI.h"
	yes | cp -rf "external/cpp/Mach1Encode.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.h"
	yes | cp -rf "external/cpp/Mach1Encode.cpp" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.cpp"
	yes | cp -rf "external/cpp/Mach1EncodeCAPI.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1EncodeCAPI.h"
	yes | cp -rf "external/cpp/Mach1Transcode.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Transcode.h"
	yes | cp -rf "external/cpp/Mach1Transcode.cpp" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Transcode.cpp"
	yes | cp -rf "external/cpp/Mach1TranscodeCAPI.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1TranscodeCAPI.h"
	yes | cp -rf "external/cpp/Mach1Point3D.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point3D.h"
	yes | cp -rf "external/cpp/Mach1Point4D.h" "$(M1SDK_RELEASE_PATH)/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point4D.h"
	echo "### DELETE all libUnity dirs"
	cd $(M1SDK_RELEASE_PATH)/mach1spatial-libs && find . -type d -name 'libUnity' -print0 | xargs -0 rm -rf
	cd $(M1SDK_RELEASE_PATH)/mach1spatial-libs && find . -type d -name 'libBundle' -print0 | xargs -0 rm -rf

deploy-dev:
	# general copy over
	echo "### Copying all successful bins to /mach1spatial-libs \###"
	yes | cp -rf "_install/" "../mach1spatial-libs/"
	# copy over mac libs
	echo "### Move and replace bins \###"
	echo "### Replacing Xcode/macOS \###"
	yes | cp -rf "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/xcode/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/xcode/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../mach1spatial-libs/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodePositionalCAPI.a"
	echo "### Replacing iOS \###"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodePositionalCAPI.a"
	echo "### Moving iOS \###"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodePositionalCAPI.a"
	echo "### Copying to CocoaPods Repo \###"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodePositionalCAPI.a"
	yes | cp -rf external/swift/* "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes/"
	echo "### Replacing Android \###"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodePositionalCAPI.a"

	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodePositionalCAPI.a"
	# Jitpack
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86_64/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86_64/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86_64/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-x86_64/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1DecodePositionalCAPI.a"
	
	yes | cp -rf external/cpp/*CAPI.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
	yes | cp -rf external/cpp/*D.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
	yes | cp -rf external/java/jni/cpp/mach1/* "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1"
	yes | cp -rf external/java/jni/java/com/mach1/spatiallibs/* "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/java/com/mach1/spatiallibs"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/mach1-decode-example/src/main/jniLibs/armeabi-v7a/libMach1DecodeCAPI.a"
	yes | cp -rf external/cpp/Mach1Decode.cpp "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
	yes | cp -rf external/cpp/Mach1Decode.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
	yes | cp -rf external/cpp/Mach1DecodeCAPI.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
	yes | cp -rf external/cpp/*D.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/mach1-decode-example/src/main/cpp"
	echo "### Copying for ofxMach1 \###"
	yes | cp -rf "external/cpp/Mach1Decode.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.h"
	yes | cp -rf "external/cpp/Mach1Decode.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.cpp"
	yes | cp -rf "external/cpp/Mach1DecodeCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodeCAPI.h"
	yes | cp -rf "external/cpp/Mach1DecodePositional.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.h"
	yes | cp -rf "external/cpp/Mach1DecodePositional.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.cpp"
	yes | cp -rf "external/cpp/Mach1DecodePositionalCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodePositionalCAPI.h"
	yes | cp -rf "external/cpp/Mach1Encode.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.h"
	yes | cp -rf "external/cpp/Mach1Encode.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.cpp"
	yes | cp -rf "external/cpp/Mach1EncodeCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1EncodeCAPI.h"
	yes | cp -rf "external/cpp/Mach1Transcode.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Transcode.h"
	yes | cp -rf "external/cpp/Mach1Transcode.cpp" "../examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Transcode.cpp"
	yes | cp -rf "external/cpp/Mach1TranscodeCAPI.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1TranscodeCAPI.h"
	yes | cp -rf "external/cpp/Mach1Point3D.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point3D.h"
	yes | cp -rf "external/cpp/Mach1Point4D.h" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point4D.h"
	echo "### Copying for UE \###"
	yes | cp -rf external/cpp/*CAPI.h "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/include/"
	yes | cp -rf external/cpp/*.h "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/Source/Mach1DecodePlugin/Public/"
	rm -f ../examples/mach1spatial-c/Unreal\ Engine/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/Source/Mach1DecodePlugin/Public/*CAPI.h
	yes | cp -rf external/cpp/*.cpp "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/Source/Mach1DecodePlugin/Private/"
	echo "### Copying for Unity \###"
	yes | cp -rf external/c#/* "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/"
	yes | cp -rf external/c#/* "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/"
	echo "### Copying libs for Unity \###"
	yes | cp -rf "../mach1spatial-libs/xcode/libBundle/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle"
	yes | cp -rf "../mach1spatial-libs/xcode/libBundle/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1EncodeCAPI.bundle"
	yes | cp -rf "../mach1spatial-libs/xcode/libBundle/libMach1TranscodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1TranscodeCAPI.bundle"
	yes | cp -rf "../mach1spatial-libs/xcode/libBundle/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodePositionalCAPI.bundle"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1EncodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1TranscodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1EncodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1TranscodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-x86/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-x86/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1EncodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-x86/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1TranscodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-x86/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "../mach1spatial-libs/xcode/libBundle/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle"
	yes | cp -rf "../mach1spatial-libs/xcode/libBundle/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1EncodeCAPI.bundle"
	yes | cp -rf "../mach1spatial-libs/xcode/libBundle/libMach1TranscodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1TranscodeCAPI.bundle"
	yes | cp -rf "../mach1spatial-libs/xcode/libBundle/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1DecodePositionalCAPI.bundle"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1DecodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1EncodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1TranscodeCAPI.a"
	yes | cp -rf "../mach1spatial-libs/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1EncodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1TranscodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-armeabi-v7a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-x86/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-x86/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1EncodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-x86/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1TranscodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-x86/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1EncodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1TranscodeCAPI.so"
	yes | cp -rf "../mach1spatial-libs/android-arm64-v8a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodePositionalCAPI.so"
	echo "### Copying ofxMach1 to dev local \###"
	yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "$(OF_PATH)/addons/"
	echo "### DELETE all libUnity dirs \###"
	cd ../mach1spatial-libs && find . -type d -name 'libUnity' -print0 | xargs -0 rm -rf
	cd ../mach1spatial-libs && find . -type d -name 'libBundle' -print0 | xargs -0 rm -rf
	echo "### NUMBERS OF SUCCESSFUL BUILDS:"
	cd ../source/_logs/polly/ && grep -rl "Installing:" "$(M1SDK_PATH)/source/_logs/polly" | wc -l
	echo "### ERRORS CAUGHT:"
	cd ../source/_logs/polly/ && grep -rl "error" "$(M1SDK_PATH)/source/_logs/polly" | wc -l
	cd ../source/_logs/polly/ && grep -rl "Error" "$(M1SDK_PATH)/source/_logs/polly" | wc -l
	cd ../source/_logs/polly/ && grep -rl "failed" "$(M1SDK_PATH)/source/_logs/polly" | wc -l
	cd ../source/_logs/polly/ && grep -rl "FAILED" "$(M1SDK_PATH)/source/_logs/polly" | wc -l

# place anything you need all commands to run here
FORCE:
