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

deploy-ios: FORCE clear
	cmake . -B_builds/ios -GXcode -DCMAKE_BUILD_TYPE=Release -DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=12 -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO -DCMAKE_IOS_INSTALL_COMBINED=YES
	cmake --build _builds/ios --config Release --target install
	cmake . -B_builds/osx -GXcode -DCMAKE_BUILD_TYPE=Release -DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON
	cmake --build _builds/osx --config Release --target install

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