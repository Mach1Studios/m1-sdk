# Mach1 Spatial SDK

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

test: FORCE clear
ifeq ($(detected_OS),Darwin)
	cd tests && ./_build_on_osx_for_osx.sh
else ifeq ($(detected_OS),Windows)
	cd tests && ./_build_on_win_for_win.bat
else
	cd tests && ./_build_on_linux_for_linux.sh
endif

test-linking: FORCE
ifeq ($(detected_OS),Darwin)
	mkdir -p builds
	g++ -o builds/spatial-decode-example ../examples/mach1spatial-c/commandline/spatial-decode-example/src/main.cpp ../mach1spatial-libs/xcode/include/Mach1Decode.cpp -I../mach1spatial-libs/xcode/include -L../mach1spatial-libs/xcode/lib -lstdc++ -lpthread -lMach1DecodeCAPI
	g++ -o builds/spatial-encode-example ../examples/mach1spatial-c/commandline/spatial-encode-example/src/main.cpp ../mach1spatial-libs/xcode/include/Mach1Encode.cpp -I../mach1spatial-libs/xcode/include -L../mach1spatial-libs/xcode/lib -lstdc++ -lpthread -lMach1EncodeCAPI
	g++ -o builds/spatial-transcode-example ../examples/mach1spatial-c/commandline/spatial-transcode-example/src/main.cpp ../mach1spatial-libs/xcode/include/Mach1Transcode.cpp -I../mach1spatial-libs/xcode/include -L../mach1spatial-libs/xcode/lib -lstdc++ -lMach1TranscodeCAPI
else ifeq ($(detected_OS),Windows)
	mkdir -p builds
	cmake ../examples/mach1spatial-c/commandline/spatial-decode-example -Bbuilds
	cmake --build builds
	cmake ../examples/mach1spatial-c/commandline/spatial-encode-example -Bbuilds
	cmake --build builds
	cmake ../examples/mach1spatial-c/commandline/spatial-transcode-example -Bbuilds
	cmake --build builds
else
	mkdir -p builds
	g++ -o builds/spatial-decode-example ../examples/mach1spatial-c/commandline/spatial-decode-example/src/main.cpp ../mach1spatial-libs/linux/include/Mach1Decode.cpp -I../mach1spatial-libs/linux/include -L../mach1spatial-libs/linux/lib -lstdc++ -pthread -lMach1DecodeCAPI
	g++ -o builds/spatial-encode-example ../examples/mach1spatial-c/commandline/spatial-encode-example/src/main.cpp ../mach1spatial-libs/linux/include/Mach1Encode.cpp -I../mach1spatial-libs/linux/include -L../mach1spatial-libs/linux/lib -lstdc++ -pthread -lMach1EncodeCAPI
	g++ -o builds/spatial-transcode-example ../examples/mach1spatial-c/commandline/spatial-transcode-example/src/main.cpp ../mach1spatial-libs/linux/include/Mach1Transcode.cpp -I../mach1spatial-libs/linux/include -L../mach1spatial-libs/linux/lib -lstdc++ -lMach1TranscodeCAPI
endif

build-all: FORCE clear test copy-public includes-dev includes-release build build-web-release build-android build-rpi build-cortexm0 deploy-dev deploy-release
	echo "REMINDER: Build FAT iOS lib first!"

build-ci-mac: FORCE clear test includes-dev includes-release build build-web-release deploy-dev deploy-release

build-ci-linux: FORCE clear test includes-dev includes-release build deploy-dev deploy-release

build-ci-win: FORCE clear test includes-dev includes-release build deploy-dev deploy-release

build: FORCE
ifeq ($(detected_OS),Darwin)
	./scripts/macos_build.sh
	#echo "[NOTE] To compile for Universal Binary follow instructions below:"
	#cat  ./macOS-UB-readme.md
	#open _builds/xcode/Mach1SpatialLibraries.xcodeproj
else ifeq ($(detected_OS),Windows)
	./scripts/win_build.sh
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

copy-public: FORCE
	rsync -c ../public/* $(M1SDK_RELEASE_PATH)

includes-dev: FORCE
	./scripts/copy_includes_dev.sh

includes-release: FORCE
	./scripts/copy_includes_release.sh

build-mac-10-13: FORCE
	./scripts/macos-10-13_build.sh

build-ios: FORCE
	./scripts/ios_build.sh

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

build-rpi: FORCE
	./scripts/rpi_build.sh

build-beaglebone: FORCE
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

build-cortexm0: FORCE
	./scripts/arm-cortexm0_build.sh

build-armhfneonvfpv4: FORCE
	./scripts/arm-hf-neon-vfpv4_build.sh

build-web-debug: FORCE
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O0 --closure 0 -s MODULARIZE=1 -gseparate-dwarf="include/js/Mach1Decode.debug.wasm" -lembind -std=c++11 -s "EXPORT_NAME='Mach1DecodeModule'" --pre-js Mach1DecodeEmscripten.js -o include/js/Mach1Decode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O0 --closure 0 -s MODULARIZE=1 -gseparate-dwarf="include/js/Mach1DecodePositional.debug.wasm" -lembind -std=c++11 -s "EXPORT_NAME='Mach1DecodePositionalModule'" -Ideps/ --pre-js Mach1DecodePositionalEmscripten.js -o include/js/Mach1DecodePositional.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodePositionalCore.cpp Mach1DecodePositionalCAPI.cpp Mach1DecodePositionalEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O0 --closure 0 -s MODULARIZE=1 -gseparate-dwarf="include/js/Mach1Encode.debug.wasm" -lembind -std=c++11 -s "EXPORT_NAME='Mach1EncodeModule'" --pre-js Mach1EncodeEmscripten.js -o include/js/Mach1Encode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O0 --closure 0 -s MODULARIZE=1 -gseparate-dwarf="include/js/Mach1Transcode.debug.wasm" -lembind -std=c++11 -s "EXPORT_NAME='Mach1TranscodeModule'" --pre-js Mach1TranscodeEmscripten.js -o include/js/Mach1Transcode.js -Iinclude -I../public -Ideps Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1TranscodeCore.cpp Mach1TranscodeCAPI.cpp Mach1TranscodeEmscripten.cpp Mach1EncodeCore.cpp Mach1GenerateCoeffs.cpp deps/M1DSP/M1DSPUtilities.cpp deps/M1DSP/M1DSPFilters.cpp

build-web-release: FORCE
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1DecodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1DecodeEmscripten.js -o include/js/Mach1Decode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1DecodePositionalModule'" -s ALLOW_TABLE_GROWTH=1 -Ideps/ --pre-js Mach1DecodePositionalEmscripten.js -o include/js/Mach1DecodePositional.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodePositionalCore.cpp Mach1DecodePositionalCAPI.cpp Mach1DecodePositionalEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1EncodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1EncodeEmscripten.js -o include/js/Mach1Encode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1TranscodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1TranscodeEmscripten.js -o include/js/Mach1Transcode.js -Iinclude -I../public -Ideps Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1TranscodeCore.cpp Mach1TranscodeCAPI.cpp Mach1TranscodeEmscripten.cpp Mach1EncodeCore.cpp Mach1GenerateCoeffs.cpp deps/M1DSP/M1DSPUtilities.cpp deps/M1DSP/M1DSPFilters.cpp
	# COPYING FOR WASM
	rsync --exclude *.debug.wasm -c include/js/* ../mach1spatial-libs/web
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

build-android: FORCE
	./scripts/android_build.sh

build-linux: FORCE
	./scripts/linux_build.sh

deploy-release: FORCE deploy-dev
	./scripts/copy_includes_release.sh
	./scripts/copy_to_examples_release.sh

deploy-dev: FORCE
	./scripts/copy_includes_dev.sh
	./scripts/copy_to_examples.sh
	# copy over mac libs
	echo "### Move and replace bins \###"
	echo "### Copying ofxMach1 to dev local \###"
	rsync -c "../examples/mach1spatial-c/openframeworks/ofxMach1" "$(OF_PATH)/addons/"
	echo "### DELETE all lib-shared dirs \###"
	cd ../mach1spatial-libs && find . -type d -name 'lib-shared' -print0 | xargs -0 rm -rf

deploy-shared-dev: FORCE deploy-dev
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
	cd ../mach1spatial-libs && find . -type d -name 'libBundle' -print0 | xargs -0 rm -rf

# place anything you need all commands to run here
FORCE:
	# local paths are in this file
	source ~/m1-globallocal.sh