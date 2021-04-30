# Mach1 Spatial SDK

EMSDK_PATH=/Volumes/git/emsdk

# getting OS type
ifeq ($(OS),Windows_NT)
	detected_OS := Windows
else
	detected_OS := $(shell uname)
endif

pull: 
	git pull --recurse-submodules

setup:
	$(EMSDK_PATH)/emsdk_env.sh

clear: 
	rm -rf _install/*

test: clear
ifeq ($(detected_OS),Darwin)
	cd tests && polly --clear --install --config Release --toolchain xcode
	./tests/_install/xcode/bin/Mach1EncodeTest
	./tests/_install/xcode/bin/Mach1DecodeTest
	./tests/_install/xcode/bin/Mach1DecodePositionalTest
	./tests/_install/xcode/bin/Mach1SpatialTests
else ifeq ($(detected_OS),Windows)
	cd tests && python ../../../polly/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64
else 
	cd tests && ~/git/polly/bin/polly --clear --install --config Release --toolchain gcc
	./tests/_install/gcc/bin/Mach1EncodeTest
	./tests/_install/gcc/bin/Mach1DecodeTest
	./tests/_install/gcc/bin/Mach1DecodePositionalTest
	./tests/_install/gcc/bin/Mach1SpatialTests
endif

test-linking:
ifeq ($(detected_OS),Darwin)

else ifeq ($(detected_OS),Windows)

else 
	g++ -o spatial-decode-exmple -I../include/cpp -L_install/gcc/lib -lMach1DecodeCAPI ../examples/mach1spatial-c/commandline/spatial-decode-example/main.cpp 
	g++ -o spatial-encode-exmple -I../include/cpp -L_install/gcc/lib -lMach1EncodeCAPI ../examples/mach1spatial-c/commandline/spatial-encode-example/main.cpp 
	g++ -o spatial-decodepositional-exmple -I../include/cpp -L_install/gcc/lib -lMach1DecodeCAPI -lMach1DecodePositionalCAPI ../examples/mach1spatial-c/commandline/spatial-decodepositional-example/main.cpp 
	g++ -o spatial-transcode-exmple -I../include/cpp -L_install/gcc/lib -lMach1TranscodeCAPI ../examples/mach1spatial-c/commandline/spatial-transcode-example/main.cpp 
endif	

build-all: clear test includes-dev includes-release build build-web grab-ios build-android build-rpi build-cortexm0 deploy-dev deploy-release
	echo "REMINDER: Build FAT iOS lib first!"

build-ci-mac: clear test includes-dev includes-release build build-web deploy-dev deploy-release

build-ci-linux: clear test includes-dev includes-release build deploy-dev deploy-release

build-ci-win: clear test includes-dev includes-release build deploy-dev deploy-release

build: FORCE
ifeq ($(detected_OS),Darwin)
	polly --clear --install --config Release --toolchain xcode
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1DecodeCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1DecodePositionalCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1EncodeCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1DecodeCAPI.bundle"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1DecodePositionalCAPI.bundle"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1EncodeCAPI.bundle"
else ifeq ($(detected_OS),Windows)
	python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-15-2017
	python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64
else 
	~/git/polly/bin/polly --clear --install --config Release --toolchain gcc
endif

includes-dev: 
	yes | cp -rf "external/" "../include/"
	mkdir -p "../include/cpp/M1DSP"
	yes | cp -rf "include/M1DSP/M1DSPUtilities.h" "../include/cpp/M1DSP/M1DSPUtilities.h"
	yes | cp -rf "include/M1DSP/M1DSPUtilities.cpp" "../include/cpp/M1DSP/M1DSPUtilities.cpp"

includes-release:
	yes | cp -rf "external/" "../../m1-sdk-release/include/"
	mkdir -p "../../m1-sdk-release/include/cpp/M1DSP"
	yes | cp -rf "include/M1DSP/M1DSPUtilities.h" "../../m1-sdk-release/include/cpp/M1DSP/M1DSPUtilities.h"
	yes | cp -rf "include/M1DSP/M1DSPUtilities.cpp" "../../m1-sdk-release/include/cpp/M1DSP/M1DSPUtilities.cpp"

build-mac:
	polly --clear --install --config Release --toolchain xcode
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1DecodeCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1DecodePositionalCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1EncodeCAPI.a"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1DecodeCAPI.bundle"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1DecodePositionalCAPI.bundle"
	codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1EncodeCAPI.bundle"
	yes | cp -rf "_install/xcode/lib/libMach1DecodeCAPI.a" "../binaries/xcode/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/xcode/lib/libMach1EncodeCAPI.a" "../binaries/xcode/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/xcode/lib/libMach1TranscodeCAPI.a" "../binaries/xcode/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/xcode/lib/libMach1DecodePositionalCAPI.a" "../binaries/xcode/lib/libMach1DecodePositionalCAPI.a"
	echo "[NOTE] To compile for Universal Binary follow instructions below:"
	cat  ./macOS-UB-readme.md
	open _builds/xcode/Mach1SpatialLibraries.xcodeproj

grab-ios:
	mkdir -p "_install/ios/lib/"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodeCAPI.a" "_install/ios/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1EncodeCAPI.a" "_install/ios/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodePositionalCAPI.a" "_install/ios/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1TranscodeCAPI.a" "_install/ios/lib/libMach1TranscodeCAPI.a"

build-ios: FORCE
	polly --clear --install --config Release --toolchain ios-12-1-dep-9-3 --ios-multiarch --ios-combined

build-ios-dev: FORCE
	polly --clear --install --config Release --toolchain ios --ios-multiarch --ios-combined

build-rpi:
	polly --clear --install --config Release --toolchain raspberrypi2-cxx11
	polly --clear --install --config Release --toolchain raspberrypi3-cxx11
	find _install/ -type d -name "libUnity" -exec rm -rf {} +
	yes | cp -rf "_install/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a" "../binaries/arm/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a" "../binaries/arm/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a" "../binaries/arm/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a" "../binaries/arm/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a" "../binaries/arm/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a" "../binaries/arm/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a" "../binaries/arm/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a" "../binaries/arm/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a"

build-beaglebone:
	echo "[NOTE] Ensure to change polly toolchain to use c++14 instead of default c++11"
	polly --clear --install --config Release --toolchain linux-gcc-armhf
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1DecodeCAPI.a" "../binaries/arm/beaglebone/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1EncodeCAPI.a" "../binaries/arm/beaglebone/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1TranscodeCAPI.a" "../binaries/arm/beaglebone/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1DecodePositionalCAPI.a" "../binaries/arm/beaglebone/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1DecodeCAPI.a" "/usr/local/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1EncodeCAPI.a" "/usr/local/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1TranscodeCAPI.a" "/usr/local/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/linux-gcc-armhf/lib/libMach1DecodePositionalCAPI.a" "/usr/local/lib/libMach1DecodePositionalCAPI.a"

build-cortexm0:
	polly --clear --install --config Release --toolchain gcc-arm-cortexm0-cxx14
	yes | cp -rf "_install/gcc-arm-cortexm0-cxx14/lib/libMach1DecodeCAPI.a" "../binaries/arm/gcc-arm-cortexm0-cxx14/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/gcc-arm-cortexm0-cxx14/lib/libMach1EncodeCAPI.a" "../binaries/arm/gcc-arm-cortexm0-cxx14/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/gcc-arm-cortexm0-cxx14/lib/libMach1TranscodeCAPI.a" "../binaries/arm/gcc-arm-cortexm0-cxx14/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/gcc-arm-cortexm0-cxx14/lib/libMach1DecodePositionalCAPI.a" "../binaries/arm/gcc-arm-cortexm0-cxx14/lib/libMach1DecodePositionalCAPI.a"

build-web: setup
	emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1DecodeModule'" --pre-js Mach1DecodeEmscripten.js -o external/js/Mach1Decode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp
	emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1DecodePositionalModule'" -Iinclude/ --pre-js Mach1DecodePositionalEmscripten.js -o external/js/Mach1DecodePositional.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodePositionalCore.cpp Mach1DecodePositionalCAPI.cpp Mach1DecodePositionalEmscripten.cpp
	emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1EncodeModule'" --pre-js Mach1EncodeEmscripten.js -o external/js/Mach1Encode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp
	emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1TranscodeModule'" --pre-js Mach1TranscodeEmscripten.js -o external/js/Mach1Transcode.js -Iinclude Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1TranscodeCore.cpp Mach1TranscodeCAPI.cpp Mach1TranscodeEmscripten.cpp Mach1GenerateCoeffs.cpp include/M1DSP/M1DSPUtilities.cpp include/M1DSP/M1DSPFilters.cpp
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
	python ../../polly/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx
	python ../../polly/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
	python ../../polly/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-clang-libcxx
	python ../../polly/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-64-clang-libcxx
ifeq ($(detected_OS),Darwin)
	yes | mv _install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx _install/android-armeabi-v7a
	yes | mv _install/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx _install/android-arm64-v8a
	yes | mv _install/android-ndk-r16b-api-21-x86-clang-libcxx _install/android-x86
	yes | mv _install/android-ndk-r16b-api-21-x86-64-clang-libcxx _install/android-x86_64
	yes | cp -rf "_install/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../binaries/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" 
	yes | cp -rf "_install/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../binaries/android-armeabi-v7a/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../binaries/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../binaries/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/android-arm64-v8a/lib/libMach1DecodeCAPI.a" "../binaries/android-arm64-v8a/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/android-arm64-v8a/lib/libMach1EncodeCAPI.a" "../binaries/android-arm64-v8a/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/android-arm64-v8a/lib/libMach1TranscodeCAPI.a" "../binaries/android-arm64-v8a/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a" "../binaries/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/android-x86/lib/libMach1DecodeCAPI.a" "../binaries/android-x86/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/android-x86/lib/libMach1EncodeCAPI.a" "../binaries/android-x86/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/android-x86/lib/libMach1TranscodeCAPI.a" "../binaries/android-x86/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/android-x86/lib/libMach1DecodePositionalCAPI.a" "../binaries/android-x86/lib/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "_install/android-x86_64/lib/libMach1DecodeCAPI.a" "../binaries/android-x86_64/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/android-x86_64/lib/libMach1EncodeCAPI.a" "../binaries/android-x86_64/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/android-x86_64/lib/libMach1TranscodeCAPI.a" "../binaries/android-x86_64/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/android-x86_64/lib/libMach1DecodePositionalCAPI.a" "../binaries/android-x86_64/lib/libMach1DecodePositionalCAPI.a"
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
	# cleanup
	rm -rf _install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
	rm -rf _install/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx
	rm -rf _install/android-ndk-r16b-api-21-x86-clang-libcxx
	rm -rf _install/android-ndk-r16b-api-21-x86-64-clang-libcxx
	find _install/ -type d -name "libUnity" -exec rm -rf {} +
else ifeq ($(detected_OS),Windows)
	# copy over android from pull
	xcopy _install\android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx\lib ..\binaries\android-arm64-v8a\lib /Y
	xcopy _install\android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx\lib ..\binaries\android-armeabi-v7a\lib /Y
	xcopy _install\android-ndk-r16b-api-21-x86-clang-libcxx\lib ..\binaries\android-x86\lib /Y
	xcopy _install\android-ndk-r16b-api-21-x86-64-clang-libcxx\lib ..\binaries\android-x86_64\lib /Y
	xcopy _install\android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx\libUnity ..\binaries\android-arm64-v8a\libUnity /Y
	xcopy _install\android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx\libUnity ..\binaries\android-armeabi-v7a\libUnity /Y
	xcopy _install\android-ndk-r16b-api-21-x86-clang-libcxx\libUnity ..\binaries\android-x86\libUnity /Y
	xcopy _install\android-ndk-r16b-api-21-x86-64-clang-libcxx\libUnity ..\binaries\android-x86_64\libUnity /Y
	del /f /s /q _android 1>nul
	rmdir /s /q _android
	mkdir ..\binaries\android-arm64-v8a
	mkdir ..\binaries\android-armeabi-v7a
	mkdir ..\binaries\android-x86
	mkdir ..\binaries\android-x86_64
	rm -rf _install/android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
	rm -rf _install/android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx
	rm -rf _install/android-ndk-r16b-api-21-x86-clang-libcxx
	rm -rf _install/android-ndk-r16b-api-21-x86-64-clang-libcxx
endif

deploy-windows:
	# copy over windows libs
	xcopy _install\vs-15-2017\lib ..\binaries\windows-x86 /Y
	xcopy _install\vs-15-2017-win64\lib ..\binaries\windows-x86_64 /Y
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

build-linux: FORCE
	cd tests && sh ./_build_on_linux_for_linux.sh
	~/git/polly/bin/polly --clear --install --config Release --toolchain gcc

deploy-linux:
	yes | cp -rf "_install/gcc/lib/libMach1DecodeCAPI.a" "../binaries/linux/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1EncodeCAPI.a" "../binaries/linux/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1TranscodeCAPI.a" "../binaries/linux/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../binaries/linux/lib/libMach1DecodePositionalCAPI.a"

build-linux-arm: FORCE
	cd tests && sh ./_build_on_linux_for_linux.sh
	~/git/polly/bin/polly --clear --install --config Release --toolchain gcc

deploy-linux-arm:
	yes | cp -rf "_install/gcc/lib/libMach1DecodeCAPI.a" "../binaries/linux-arm/lib/libMach1DecodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1EncodeCAPI.a" "../binaries/linux-arm/lib/libMach1EncodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1TranscodeCAPI.a" "../binaries/linux-arm/lib/libMach1TranscodeCAPI.a"
	yes | cp -rf "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../binaries/linux-arm/lib/libMach1DecodePositionalCAPI.a"

deploy-release: deploy-dev
	#replace all binaries in github release 
	echo "### Copying all successful bins to /Binaries \###"
	yes | cp -rf "_install/" "../../m1-sdk-release/binaries/"
	echo "### Move and replace bins \###"
	echo "### Replacing Xcode/macOS \###"
	yes | cp -rf "../binaries/xcode/lib/libMach1DecodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/xcode/lib/libMach1EncodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/xcode/lib/libMach1TranscodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/xcode/lib/libMach1DecodePositionalCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodePositionalCAPI.a"
	echo "### Moving iOS \###"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1EncodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1TranscodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodePositionalCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodePositionalCAPI.a"
	echo "### Replacing Android \###"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1DecodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1EncodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1TranscodeCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1DecodePositionalCAPI.a" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodePositionalCAPI.a"
	echo "### Copying for ofxMach1 \###"
	yes | cp -rf "external/cpp/Mach1Decode.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.h"
	yes | cp -rf "external/cpp/Mach1Decode.cpp" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Decode.cpp"
	yes | cp -rf "external/cpp/Mach1DecodeCAPI.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodeCAPI.h"
	yes | cp -rf "external/cpp/Mach1DecodePositional.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.h"
	yes | cp -rf "external/cpp/Mach1DecodePositional.cpp" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1DecodePositional.cpp"
	yes | cp -rf "external/cpp/Mach1DecodePositionalCAPI.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1DecodePositionalCAPI.h"
	yes | cp -rf "external/cpp/Mach1Encode.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.h"
	yes | cp -rf "external/cpp/Mach1Encode.cpp" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Encode.cpp"
	yes | cp -rf "external/cpp/Mach1EncodeCAPI.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1EncodeCAPI.h"
	yes | cp -rf "external/cpp/Mach1Transcode.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Transcode.h"
	yes | cp -rf "external/cpp/Mach1Transcode.cpp" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/src/Mach1Transcode.cpp"
	yes | cp -rf "external/cpp/Mach1TranscodeCAPI.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1TranscodeCAPI.h"
	yes | cp -rf "external/cpp/Mach1Point3D.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point3D.h"
	yes | cp -rf "external/cpp/Mach1Point4D.h" "../../m1-sdk-release/examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/include/Mach1Point4D.h"
	echo "### DELETE all libUnity dirs"
	cd ../../m1-sdk-release/binaries && find . -type d -name 'libUnity' -print0 | xargs -0 rm -rf
	cd ../../m1-sdk-release/binaries && find . -type d -name 'libBundle' -print0 | xargs -0 rm -rf

deploy-dev:
	# general copy over
	echo "### Copying all successful bins to /Binaries \###"
	yes | cp -rf "_install/" "../binaries/"
	# copy over mac libs
	echo "### Move and replace bins \###"
	echo "### Replacing Xcode/macOS \###"
	yes | cp -rf "../binaries/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/xcode/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/xcode/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/osx/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../binaries/xcode/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/xcode/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Mac/libMach1DecodePositionalCAPI.a"
	echo "### Replacing iOS \###"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/IOS/libMach1DecodePositionalCAPI.a"
	echo "### Moving iOS \###"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/ios/libMach1DecodePositionalCAPI.a"
	echo "### Copying to CocoaPods Repo \###"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/ios/libMach1DecodePositionalCAPI.a"
	yes | cp -rf external/swift/* "../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Classes/"
	echo "### Replacing Android \###"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/armeabi-v7a/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/armeabi-v7a/libMach1DecodePositionalCAPI.a"

	yes | cp -rf "../binaries/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Android/x86/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/android/x86/libMach1DecodePositionalCAPI.a"
	# Jitpack
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/armeabi-v7a/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../binaries/android-arm64-v8a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/android-arm64-v8a/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/android-arm64-v8a/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/android-arm64-v8a/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/arm64-v8a/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/android-x86/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../binaries/android-x86_64/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/android-x86_64/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/android-x86_64/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/android-x86_64/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/jniLibs/x86_64/libMach1DecodePositionalCAPI.a"
	
	yes | cp -rf external/cpp/*CAPI.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
	yes | cp -rf external/cpp/*D.h "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/"
	yes | cp -rf external/java/jni/cpp/mach1/* "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1"
	yes | cp -rf external/java/jni/java/com/mach1/spatiallibs/* "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/java/com/mach1/spatiallibs"
	yes | cp -rf "../binaries/android-armeabi-v7a/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/mach1-decode-example/src/main/jniLibs/armeabi-v7a/libMach1DecodeCAPI.a"
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
	yes | cp -rf "../binaries/xcode/libBundle/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle"
	yes | cp -rf "../binaries/xcode/libBundle/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1EncodeCAPI.bundle"
	yes | cp -rf "../binaries/xcode/libBundle/libMach1TranscodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1TranscodeCAPI.bundle"
	yes | cp -rf "../binaries/xcode/libBundle/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS/libMach1DecodePositionalCAPI.bundle"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/iOS/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so"
	yes | cp -rf "../binaries/android-armeabi-v7a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1EncodeCAPI.so"
	yes | cp -rf "../binaries/android-armeabi-v7a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1TranscodeCAPI.so"
	yes | cp -rf "../binaries/android-armeabi-v7a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "../binaries/android-arm64-v8a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodeCAPI.so"
	yes | cp -rf "../binaries/android-arm64-v8a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1EncodeCAPI.so"
	yes | cp -rf "../binaries/android-arm64-v8a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1TranscodeCAPI.so"
	yes | cp -rf "../binaries/android-arm64-v8a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "../binaries/android-x86/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
	yes | cp -rf "../binaries/android-x86/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1EncodeCAPI.so"
	yes | cp -rf "../binaries/android-x86/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1TranscodeCAPI.so"
	yes | cp -rf "../binaries/android-x86/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Android/x86/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "../binaries/xcode/libBundle/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle"
	yes | cp -rf "../binaries/xcode/libBundle/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1EncodeCAPI.bundle"
	yes | cp -rf "../binaries/xcode/libBundle/libMach1TranscodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1TranscodeCAPI.bundle"
	yes | cp -rf "../binaries/xcode/libBundle/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS/libMach1DecodePositionalCAPI.bundle"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1DecodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1EncodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1EncodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1TranscodeCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1TranscodeCAPI.a"
	yes | cp -rf "../binaries/ios/lib/libMach1DecodePositionalCAPI.a" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/iOS/libMach1DecodePositionalCAPI.a"
	yes | cp -rf "../binaries/android-armeabi-v7a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodeCAPI.so"
	yes | cp -rf "../binaries/android-armeabi-v7a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1EncodeCAPI.so"
	yes | cp -rf "../binaries/android-armeabi-v7a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1TranscodeCAPI.so"
	yes | cp -rf "../binaries/android-armeabi-v7a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/armeabi-v7a/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "../binaries/android-x86/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1DecodeCAPI.so"
	yes | cp -rf "../binaries/android-x86/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1EncodeCAPI.so"
	yes | cp -rf "../binaries/android-x86/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1TranscodeCAPI.so"
	yes | cp -rf "../binaries/android-x86/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/x86/libMach1DecodePositionalCAPI.so"
	yes | cp -rf "../binaries/android-arm64-v8a/libUnity/libMach1DecodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodeCAPI.so"
	yes | cp -rf "../binaries/android-arm64-v8a/libUnity/libMach1EncodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1EncodeCAPI.so"
	yes | cp -rf "../binaries/android-arm64-v8a/libUnity/libMach1TranscodeCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1TranscodeCAPI.so"
	yes | cp -rf "../binaries/android-arm64-v8a/libUnity/libMach1DecodePositionalCAPI.so" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Android/arm64-v8a/libMach1DecodePositionalCAPI.so"
	echo "### Copying ofxMach1 to dev local \###"
	yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../openFrameworks/addons/"
	yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../openFrameworks-git/addons/"
	yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../of_v0.9.8_osx_release/addons/"
	yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../of_v0.10.1_osx_release/addons/"
	yes | cp -rf "../examples/mach1spatial-c/openframeworks/ofxMach1" "../../of_v0.11.0_osx_release/addons/"
	echo "### DELETE all libUnity dirs \###"
	cd ../binaries && find . -type d -name 'libUnity' -print0 | xargs -0 rm -rf
	cd ../binaries && find . -type d -name 'libBundle' -print0 | xargs -0 rm -rf
	echo "### NUMBERS OF SUCCESSFUL BUILDS:"
	cd ../lib/_logs/polly/ && grep -rl "Installing:" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
	echo "### ERRORS CAUGHT:"
	cd ../lib/_logs/polly/ && grep -rl "error" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
	cd ../lib/_logs/polly/ && grep -rl "Error" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
	cd ../lib/_logs/polly/ && grep -rl "failed" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l
	cd ../lib/_logs/polly/ && grep -rl "FAILED" "/Volumes/git/m1-sdk/lib/_logs/polly" | wc -l

# place anything you need all commands to run here
FORCE: