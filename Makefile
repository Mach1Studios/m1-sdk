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

build-all: FORCE clear test copy-public includes-dev includes-release build build-web-release build-ndkr16b-android build-ndkr26d-android build-rpi build-cortexm0 deploy-dev deploy-release
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

includes-dev: FORCE
	./scripts/copy_includes_dev.sh

build-web-debug: FORCE
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O0 --closure 0 -s MODULARIZE=1 -gseparate-dwarf="include/js/Mach1Decode.debug.wasm" -lembind -std=c++11 -s "EXPORT_NAME='Mach1DecodeModule'" --pre-js Mach1DecodeEmscripten.js -o include/js/Mach1Decode.js Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O0 --closure 0 -s MODULARIZE=1 -gseparate-dwarf="include/js/Mach1DecodePositional.debug.wasm" -lembind -std=c++11 -s "EXPORT_NAME='Mach1DecodePositionalModule'" -Ideps/ --pre-js Mach1DecodePositionalEmscripten.js -o include/js/Mach1DecodePositional.js Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodePositionalCore.cpp Mach1DecodePositionalCAPI.cpp Mach1DecodePositionalEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O0 --closure 0 -s MODULARIZE=1 -gseparate-dwarf="include/js/Mach1Encode.debug.wasm" -lembind -std=c++11 -s "EXPORT_NAME='Mach1EncodeModule'" --pre-js Mach1EncodeEmscripten.js -o include/js/Mach1Encode.js Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O0 --closure 0 -s MODULARIZE=1 -gseparate-dwarf="include/js/Mach1Transcode.debug.wasm" -lembind -std=c++11 -s "EXPORT_NAME='Mach1TranscodeModule'" --pre-js Mach1TranscodeEmscripten.js -o include/js/Mach1Transcode.js -Iinclude -I../public -Ideps Mach1TranscodeCore.cpp Mach1TranscodeCAPI.cpp Mach1TranscodeEmscripten.cpp Mach1EncodeCore.cpp Mach1GenerateCoeffs.cpp deps/M1DSP/M1DSPUtilities.cpp deps/M1DSP/M1DSPFilters.cpp

build-web-release: FORCE
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1DecodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1DecodeEmscripten.js -o include/js/Mach1Decode.js Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1DecodePositionalModule'" -s ALLOW_TABLE_GROWTH=1 -Ideps/ --pre-js Mach1DecodePositionalEmscripten.js -o include/js/Mach1DecodePositional.js Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodePositionalCore.cpp Mach1DecodePositionalCAPI.cpp Mach1DecodePositionalEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1EncodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1EncodeEmscripten.js -o include/js/Mach1Encode.js Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp
	source $(EMSDK_PATH)/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind -s "EXPORT_NAME='Mach1TranscodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1TranscodeEmscripten.js -o include/js/Mach1Transcode.js -Iinclude -I../public -Ideps Mach1TranscodeCore.cpp Mach1TranscodeCAPI.cpp Mach1TranscodeEmscripten.cpp Mach1EncodeCore.cpp Mach1GenerateCoeffs.cpp deps/M1DSP/M1DSPUtilities.cpp deps/M1DSP/M1DSPFilters.cpp
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

build-ndkr16b-android: FORCE
	./scripts/android-ndkr16b_build.sh

build-ndkr26d-android: FORCE
	./scripts/android-ndkr26d_build.sh

deploy: FORCE
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
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS"
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS"
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1TranscodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS"
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS"
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
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1DecodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS"
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1EncodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS"
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1TranscodeCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS"
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1DecodePositionalCAPI.bundle" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/macOS"
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
	cd ../mach1spatial-libs && find . -type d -name 'lib-bundle' -print0 | xargs -0 rm -rf

dev-unity: FORCE clear build
	echo "### Copying libs for Test Project \###"
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1DecodeCAPI.bundle" ~/unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1EncodeCAPI.bundle" ~/unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1TranscodeCAPI.bundle" ~/unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS
	rsync -rc "../mach1spatial-libs/xcode/lib-bundle/libMach1DecodePositionalCAPI.bundle" ~/unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/macOS

# place anything you need all commands to run here
FORCE: