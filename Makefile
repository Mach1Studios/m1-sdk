# Mach1 Spatial SDK

# Make sure you fill all the variables and paths
include ./Makefile.variables

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
ifeq ($(detected_OS),Windows)
	if exist _install del /f /q _install
	if exist _builds del /f /q _builds
else
	rm -rf _install
	rm -rf _builds
endif

# use cmake_generator="" to specify generator.
test: FORCE clear
ifeq ($(detected_OS),Darwin)
	cd tests && ./_build_on_osx_for_osx.sh
else ifeq ($(detected_OS),Windows)
	@set "cmake_generator=$(cmake_generator)"
	cd tests && _build_on_win_for_win.bat 
else
	cd tests && ./_build_on_linux_for_linux.sh
endif

build: FORCE
ifeq ($(detected_OS),Darwin)
	cmake . -B_builds/osx -GXcode -DCMAKE_BUILD_TYPE=Release
	cmake --build _builds/osx --config Release
else ifeq ($(detected_OS),Windows)
endif

deploy-android: FORCE clear
	# BUILD arm64
	cmake . -B_builds/android-arm64-v8a \
	-DM1S_BUILD_EXAMPLES=OFF -BUILD_JITPACK_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_SYSTEM_NAME=Android \
	-DCMAKE_SYSTEM_VERSION=21 \
	-DCMAKE_ANDROID_ARCH_ABI=arm64-v8a \
	-DCMAKE_ANDROID_NDK=$ANDROID_NDK_HOME \
	-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DBUILD_SHARED_LIBS=ON \
	-DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-arm64-v8a
	cmake --build _builds/android-arm64-v8a --config Release --target install
	# BUILD armeabi-v7a
	cmake . -B_builds/android-armeabi-v7a \
	-DM1S_BUILD_EXAMPLES=OFF -BUILD_JITPACK_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_SYSTEM_NAME=Android \
	-DCMAKE_SYSTEM_VERSION=21 \
	-DCMAKE_ANDROID_ARCH=armeabi-v7a \
	-DCMAKE_ANDROID_NDK=$ANDROID_NDK_HOME \
	-DCMAKE_ANDROID_ARM_NEON=ON \
	-DCMAKE_ANDROID_ARM_MODE=ON \
	-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DBUILD_SHARED_LIBS=ON \
	-DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-armeabi-v7a
	cmake --build _builds/android-armeabi-v7a --config Release --target install
	# BUILD x86
	cmake . -B_builds/android-x86 \
	-DM1S_BUILD_EXAMPLES=OFF -BUILD_JITPACK_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_SYSTEM_NAME=Android \
	-DCMAKE_SYSTEM_VERSION=21 \
	-DCMAKE_ANDROID_ARCH=x86 \
	-DCMAKE_ANDROID_NDK=$ANDROID_NDK_HOME \
	-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DBUILD_SHARED_LIBS=ON \
	-DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-x86
	cmake --build _builds/android-x86 --config Release --target install
	# BUILD x64
	cmake . -B_builds/android-x86-64 \
	-DM1S_BUILD_EXAMPLES=OFF -BUILD_JITPACK_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_SYSTEM_NAME=Android \
	-DCMAKE_SYSTEM_VERSION=21 \
	-DCMAKE_ANDROID_ARCH=x86_64 \
	-DCMAKE_ANDROID_NDK=$ANDROID_NDK_HOME \
	-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DBUILD_SHARED_LIBS=ON \
	-DCMAKE_INSTALL_PREFIX=`pwd`/_install/android-x86-64
	cmake --build _builds/android-x86-64 --config Release --target install

deploy-ios: FORCE clear
ifeq ($(detected_OS),Darwin)
	cmake . -B_builds/osx \
	-GXcode \
	-DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=MAC_UNIVERSAL
	cmake --build _builds/osx --config Release --target install
	cmake . -B_builds/ios \
	-GXcode \
	-DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=OS64COMBINED
	cmake --build _builds/ios --config Release # separate build and install steps for fat-lib
	cmake --install _builds/ios --config Release
endif

deploy-web: FORCE clear
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1DecodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1DecodeEmscripten.js -o include/js/Mach1Decode.js Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1DecodePositionalModule'" -s ALLOW_TABLE_GROWTH=1 -Ideps/ --pre-js Mach1DecodePositionalEmscripten.js -o include/js/Mach1DecodePositional.js Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodePositionalCore.cpp Mach1DecodePositionalCAPI.cpp Mach1DecodePositionalEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1EncodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1EncodeEmscripten.js -o include/js/Mach1Encode.js Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1TranscodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1TranscodeEmscripten.js -o include/js/Mach1Transcode.js -Iinclude -I../public -Ideps Mach1TranscodeCore.cpp Mach1TranscodeCAPI.cpp Mach1TranscodeEmscripten.cpp Mach1EncodeCore.cpp Mach1GenerateCoeffs.cpp deps/M1DSP/M1DSPUtilities.cpp deps/M1DSP/M1DSPFilters.cpp
	# NODEJS
	rsync -c "libmach1spatial/include/js/Mach1Decode.js" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1Decode.js"
	rsync -c "libmach1spatial/include/js/Mach1Decode.wasm" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1Decode.wasm"
	rsync -c "libmach1spatial/include/js/Mach1DecodePositional.js" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1DecodePositional.js"
	rsync -c "libmach1spatial/include/js/Mach1DecodePositional.wasm" "../examples/mach1spatial-nodejs/mach1spatial-decode/lib/Mach1DecodePositional.wasm"
	rsync -c "libmach1spatial/include/js/Mach1Encode.js" "../examples/mach1spatial-nodejs/mach1spatial-encode/lib/Mach1Encode.js"
	rsync -c "libmach1spatial/include/js/Mach1Encode.wasm" "../examples/mach1spatial-nodejs/mach1spatial-encode/lib/Mach1Encode.wasm"
	rsync -c "libmach1spatial/include/js/Mach1Transcode.js" "../examples/mach1spatial-nodejs/mach1spatial-transcode/lib/Mach1Transcode.js"
	rsync -c "libmach1spatial/include/js/Mach1Transcode.wasm" "../examples/mach1spatial-nodejs/mach1spatial-transcode/lib/Mach1Transcode.wasm"
	# EXAMPLES
	rsync -c libmach1spatial/include/js/* ../examples/mach1spatial-web/js
	rsync -c "libmach1spatial/include/js/Mach1Decode.js" "../examples/mach1spatial-web/m1-web-spatialaudioplayer/js/Mach1Decode.js"
	rsync -c "libmach1spatial/include/js/Mach1Decode.wasm" "../examples/mach1spatial-web/m1-web-spatialaudioplayer/js/Mach1Decode.wasm"
	# OFXMACH1
	rsync -c  --exclude '*CAPI.h' libmach1spatial/include/include/cpp/* ../examples/mach1spatial-c/openframeworks/ofxMach1/src
	rsync -c libmach1spatial/include/js/* ../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/emscripten/

# place anything you need all commands to run here
FORCE: