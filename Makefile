# Mach1 Spatial SDK

# getting OS type
ifeq ($(OS),Windows_NT)
	detected_OS := Windows
else
	detected_OS := $(shell uname)
endif

pull:
	git pull --recurse-submodules

# Windows: requires cmake and pip to be installed first
setup:
ifeq ($(detected_OS),Windows)
	@if not exist "$(VCPKG_ROOT)\vcpkg.exe" (echo "vcpkg is not installed, if it is add VCPKG_ROOT with '$env:VCPKG_ROOT = C:/path/to/vcpkg' and then '$env:PATH = \"$env:VCPKG_ROOT;$env:PATH\"'" && exit 1)
	@vcpkg version >nul || (echo "vcpkg is not working" && exit 1)
	@echo "vcpkg is installed and working"
	vcpkg install rtaudio libvorbis:x64-windows-static libflac:x64-windows-static opus:x64-windows-static
	@if not exist "$(pip show pre-commit)" (pip install pre-commit)
	pre-commit install
	@choco version >nul || (echo "choco is not installed" && exit 1)
	@echo "choco is installed and working"
	choco install doxygen.install
else ifeq ($(detected_OS),Darwin)
	brew install cmake emscripten pre-commit libsndfile rtaudio doxygen
	pre-commit install
else
	sudo apt-add-repository universe && sudo apt-get update && sudo apt install libsndfile-dev cmake emscripten librtaudio-dev pre-commit doxygen
	pre-commit install
endif

clean:
ifeq ($(detected_OS),Windows)
	@if exist _builds (rmdir /s /q _builds)
else
	rm -rf _builds
endif

doxygen:
	cd docs && doxygen Doxyfile

# use cmake_generator="" to specify generator.
test: clean
ifeq ($(detected_OS),Darwin)
	tests/_build_on_osx_for_osx.sh
else ifeq ($(detected_OS),Windows)
	.\\tests\\_build_on_win_for_win.bat
else
	tests/_build_on_linux_for_linux.sh
endif

build: clean
ifeq ($(detected_OS),Darwin)
	cmake . -B_builds/xcode -GXcode -DM1S_BUILD_TESTS=ON -DM1S_BUILD_EXAMPLES=ON -DM1S_BUILD_SIGNAL_SUITE=ON
	cmake --build _builds/xcode --config Release
else ifeq ($(detected_OS),Windows)
	if defined cmake_generator (
	    cmake . -B_builds/windows-x86_64 -G %cmake_generator% -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" -DM1S_BUILD_TESTS=ON -DM1S_BUILD_EXAMPLES=ON -DM1S_BUILD_SIGNAL_SUITE=ON
	) else (
	    cmake . -B_builds/windows-x86_64 -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" -DM1S_BUILD_TESTS=ON -DM1S_BUILD_EXAMPLES=ON -DM1S_BUILD_SIGNAL_SUITE=ON
	)
	cmake --build _builds/windows-x86_64 --config "Release"
endif

deploy-android: clean generate-jni-wrapper
	# Using CMake install since we will be building a lib instead of copying source code
	# TODO: Create jni source files
	# BUILD arm64
	cmake . -B_builds/android-arm64-v8a \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_JITPACK_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake \
	-DCMAKE_SYSTEM_NAME=Android \
	-DANDROID_PLATFORM=21 \
	-DANDROID_ABI=arm64-v8a \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DCMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK}
	cmake --build _builds/android-arm64-v8a --config Release --target install
	# BUILD armeabi-v7a
	cmake . -B_builds/android-armeabi-v7a \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_JITPACK_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake \
	-DCMAKE_SYSTEM_NAME=Android \
	-DANDROID_PLATFORM=21 \
	-DANDROID_ABI=armeabi-v7a \
	-DCMAKE_ANDROID_ARM_NEON=ON \
	-DCMAKE_ANDROID_ARM_MODE=ON \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DCMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK}
	cmake --build _builds/android-armeabi-v7a --config Release --target install
	# BUILD x86
	cmake . -B_builds/android-x86 \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_JITPACK_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake \
	-DCMAKE_SYSTEM_NAME=Android \
	-DANDROID_PLATFORM=21 \
	-DANDROID_ABI=x86 \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DCMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK}
	cmake --build _builds/android-x86 --config Release --target install
	# BUILD x64
	cmake . -B_builds/android-x86-64 \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_JITPACK_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake \
	-DCMAKE_SYSTEM_NAME=Android \
	-DANDROID_PLATFORM=21 \
	-DANDROID_ABI=x86_64 \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DCMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK}
	cmake --build _builds/android-x86-64 --config Release --target install

deploy-ios: clean
ifeq ($(detected_OS),Darwin)
	# Using CMake install since we will be building a lib instead of copying source code
	# TODO: Create .swift source files
	cmake . -B_builds/osx \
	-GXcode \
	-DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=MAC_UNIVERSAL
	cmake --build _builds/osx --config Release
	cmake --install _builds/osx --config Release
	cmake . -B_builds/ios \
	-GXcode \
	-DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=OS64COMBINED
	cmake --build _builds/ios --config Release # separate build and install steps for fat-lib
	cmake --install _builds/ios --config Release
	# TODO: test the libs are codesigned and built properly
endif

deploy-web: generate-js
	# NODEJS
	rsync -c libmach1spatial/api_decode/include/js/* examples/mach1spatial-nodejs/mach1spatial-decode/lib
	rsync -c libmach1spatial/api_decodepositional/include/js/* examples/mach1spatial-nodejs/mach1spatial-decode/lib
	rsync -c libmach1spatial/api_encode/include/js/* examples/mach1spatial-nodejs/mach1spatial-encode/lib
	rsync -c libmach1spatial/api_transcode/include/js/* examples/mach1spatial-nodejs/mach1spatial-transcode/lib
	# EXAMPLES
	rsync -c libmach1spatial/api_*/include/js/* examples/mach1spatial-web/js
	rsync -c libmach1spatial/api_decode/include/js/* examples/mach1spatial-web/m1-web-spatialaudioplayer/js
	# OFXMACH1
	rsync -c libmach1spatial/api_*/include/js/* examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/emscripten/

deploy-ue: clean build
	# api_common
	rsync -c libmach1spatial/api_common/include/Mach1Point*.h examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Source/Mach1DecodeAPI/Public
	# api_decode
	rsync -c libmach1spatial/api_decode/src/*.cpp examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Source/Mach1DecodeAPI/Private --exclude='js/' --exclude='*.swift' --exclude='*.cs' --exclude='*Emscripten*'
	rsync -c libmach1spatial/api_decode/include/Mach1DecodeCAPI.h examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Source/Mach1DecodeAPI/Public
	rsync -c libmach1spatial/api_decode/src/Mach1DecodeCore.h examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Source/Mach1DecodeAPI/Public
	rsync -c libmach1spatial/api_decode/include/Mach1Decode.cpp  examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Source/Mach1DecodePlugin/Private
	rsync -c libmach1spatial/api_decode/include/Mach1Decode.h  examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Source/Mach1DecodePlugin/Public
	# api_decodepositional
	rsync -c libmach1spatial/api_decodepositional/src/*.cpp examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Source/Mach1DecodeAPI/Private --exclude='js/' --exclude='*.swift' --exclude='*.cs' --exclude='*Emscripten*'
	rsync -c libmach1spatial/api_decodepositional/include/Mach1DecodePositionalCAPI.h examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Source/Mach1DecodeAPI/Public
	rsync -c libmach1spatial/api_decodepositional/src/Mach1DecodePositionalCore.h examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Source/Mach1DecodeAPI/Public
	rsync -c libmach1spatial/api_decodepositional/include/Mach1DecodePositional.cpp  examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Source/Mach1DecodePlugin/Private
	rsync -c libmach1spatial/api_decodepositional/include/Mach1DecodePositional.h  examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Source/Mach1DecodePlugin/Public

deploy-unity: clean build
	# api_common
	rsync -c libmach1spatial/api_common/include examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/libMach1Spatial/api_common --exclude='*AudioTimeline*' --exclude='*Mach1KeyPoint.h'
	# api_decode
	rsync -c libmach1spatial/api_decode/include examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/libMach1Spatial/api_decode --exclude='js/' --exclude='*.swift' --exclude='*Emscripten*'
	rsync -c libmach1spatial/api_decode/src examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/libMach1Spatial/api_decode --exclude='js/' --exclude='*.swift' --exclude='*Emscripten*'
	# api_decodepositional
	rsync -c libmach1spatial/api_decodepositional/include examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/libMach1Spatial/api_decodepositional --exclude='js/' --exclude='*.swift' --exclude='*Emscripten*'
	rsync -c libmach1spatial/api_decodepositional/src examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/libMach1Spatial/api_decodepositional --exclude='js/' --exclude='*.swift' --exclude='*Emscripten*'

generate-jni-wrapper:
	mkdir -p libmach1spatial/swig/jni/java/com/mach1/spatiallibs
	swig -c++ -java \
	-outdir libmach1spatial/swig/jni/java/com/mach1/spatiallibs \
	-Ilibmach1spatial/api_common \
	-package com.mach1.spatiallibs libmach1spatial/swig/Mach1FloatArrayModule.i
	swig -c++ -java \
	-outdir libmach1spatial/swig/jni/java/com/mach1/spatiallibs \
	-Ilibmach1spatial/api_common/swift \
	-package com.mach1.spatiallibs libmach1spatial/swig/Mach1Point3DModule.i
	swig -c++ -java \
	-outdir libmach1spatial/swig/jni/java/com/mach1/spatiallibs \
	-Ilibmach1spatial/api_common/swift \
	-package com.mach1.spatiallibs libmach1spatial/swig/Mach1Point4DModule.i
	swig -c++ -java \
	-outdir libmach1spatial/swig/jni/java/com/mach1/spatiallibs \
	-Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	-package com.mach1.spatiallibs libmach1spatial/swig/Mach1DecodeModule.i
	swig -c++ -java \
	-outdir libmach1spatial/swig/jni/java/com/mach1/spatiallibs \
	-Ilibmach1spatial/api_encode/include -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	-package com.mach1.spatiallibs libmach1spatial/swig/Mach1EncodeModule.i
	swig -c++ -java \
	-outdir libmach1spatial/swig/jni/java/com/mach1/spatiallibs \
	-Ilibmach1spatial/api_transcode/include -Ilibmach1spatial/api_common/swift \
	-package com.mach1.spatiallibs libmach1spatial/swig/Mach1TranscodeModule.i
	swig -c++ -java \
	-outdir libmach1spatial/swig/jni/java/com/mach1/spatiallibs \
	-Ilibmach1spatial/api_decodepositional/include -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	-package com.mach1.spatiallibs libmach1spatial/swig/Mach1DecodePositionalModule.i

generate-csharp-wrapper:
	mkdir -p libmach1spatial/swig/csharp
	swig -csharp \
	-outdir libmach1spatial/swig/csharp \
	-Ilibmach1spatial/api_common \
	libmach1spatial/swig/Mach1FloatArrayModule.i
	swig -csharp \
	-outdir libmach1spatial/swig/csharp \
	-Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Point3DModule.i
	swig -csharp \
	-outdir libmach1spatial/swig/csharp \
	-Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Point4DModule.i
	swig -csharp \
	-outdir libmach1spatial/swig/csharp \
	-Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1DecodeModule.i
	swig -csharp \
	-outdir libmach1spatial/swig/csharp \
	-Ilibmach1spatial/api_encode/include -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1EncodeModule.i
	swig -csharp \
	-outdir libmach1spatial/swig/csharp \
	-Ilibmach1spatial/api_transcode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1TranscodeModule.i
	swig -csharp \
	-outdir libmach1spatial/swig/csharp \
	-Ilibmach1spatial/api_decodepositional/include -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1DecodePositionalModule.i

generate-csharp:
	mkdir -p libmach1spatial/swig/cs
	swig -csharp \
	-outdir libmach1spatial/swig/cs \
	-Ilibmach1spatial/api_common \
	libmach1spatial/swig/Mach1FloatArrayModule.i
	swig -csharp \
	-outdir libmach1spatial/swig/cs \
	-Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Point3DModule.i
	swig -csharp \
	-outdir libmach1spatial/swig/cs \
	-Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Point4DModule.i
	swig -csharp \
	-outdir libmach1spatial/swig/cs \
	-Ilibmach1spatial/api_decode/src -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Decode.i
	swig -csharp \
	-outdir libmach1spatial/swig/cs \
	-Ilibmach1spatial/api_encode/src -Ilibmach1spatial/api_encode/include -Ilibmach1spatial/api_decode/src -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Encode.i
	swig -csharp \
	-outdir libmach1spatial/swig/cs \
	-Ilibmach1spatial/api_transcode/src -Ilibmach1spatial/api_transcode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Transcode.i
	swig -csharp \
	-outdir libmach1spatial/swig/cs \
	-Ilibmach1spatial/api_decodepositional/src -Ilibmach1spatial/api_decodepositional/include -Ilibmach1spatial/api_decode/src -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1DecodePositional.i

generate-python:
	mkdir -p libmach1spatial/swig/python
	swig -python \
	-outdir libmach1spatial/swig/python \
	-Ilibmach1spatial/api_common \
	libmach1spatial/swig/Mach1FloatArrayModule.i
	swig -python \
	-outdir libmach1spatial/swig/python \
	-Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Point3D.i
	swig -python \
	-outdir libmach1spatial/swig/python \
	-Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Point4D.i
	swig -python \
	-outdir libmach1spatial/swig/python \
	-Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Decode.i
	swig -python \
	-outdir libmach1spatial/swig/python \
	-Ilibmach1spatial/api_encode/include -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Encode.i
	swig -python \
	-outdir libmach1spatial/swig/python \
	-Ilibmach1spatial/api_transcode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Transcode.i
	swig -python \
	-outdir libmach1spatial/swig/python \
	-Ilibmach1spatial/api_decodepositional/include -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1DecodePositional.i

generate-go:
	mkdir -p libmach1spatial/swig/go
	swig -go \
	-outdir libmach1spatial/swig/go \
	-Ilibmach1spatial/api_common \
	libmach1spatial/swig/Mach1FloatArrayModule.i
	swig -go \
	-outdir libmach1spatial/swig/go \
	-Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Point3D.i
	swig -go \
	-outdir libmach1spatial/swig/go \
	-Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Point4D.i
	swig -go \
	-outdir libmach1spatial/swig/go \
	-Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Decode.i
	swig -go \
	-outdir libmach1spatial/swig/go \
	-Ilibmach1spatial/api_encode/include -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Encode.i
	swig -go \
	-outdir libmach1spatial/swig/go \
	-Ilibmach1spatial/api_transcode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1Transcode.i
	swig -go \
	-outdir libmach1spatial/swig/go \
	-Ilibmach1spatial/api_decodepositional/include -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_common/swift \
	libmach1spatial/swig/Mach1DecodePositional.i

generate-js:
	emcc -O3 --closure 0 --minify 0 -s MODULARIZE=1 --bind -s ALLOW_TABLE_GROWTH=1 \
	-s "EXPORT_NAME='Mach1DecodeModule'" \
	--pre-js libmach1spatial/api_decode/src/Mach1DecodeEmscripten.js \
	-Ilibmach1spatial/api_common/include -Ilibmach1spatial/api_decode/include \
	libmach1spatial/api_decode/src/Mach1DecodeCore.cpp libmach1spatial/api_decode/src/Mach1DecodeCAPI.cpp libmach1spatial/api_decode/src/Mach1DecodeEmscripten.cpp \
	-o libmach1spatial/api_decode/include/js/Mach1Decode.js
	emcc -O3 --closure 0 --minify 0 -s MODULARIZE=1 --bind -s ALLOW_TABLE_GROWTH=1 \
	-s "EXPORT_NAME='Mach1DecodePositionalModule'" \
	--pre-js libmach1spatial/api_decodepositional/src/Mach1DecodePositionalEmscripten.js \
	-Ilibmach1spatial/api_decodepositional/include -Ilibmach1spatial/api_common/include -Ilibmach1spatial/api_decode/include -Ilibmach1spatial/api_decode/src -Ilibmach1spatial/deps/ -Ilibmach1spatial/deps/glm \
	libmach1spatial/api_decode/src/Mach1DecodeCore.cpp libmach1spatial/api_decode/src/Mach1DecodeCAPI.cpp libmach1spatial/api_decodepositional/src/Mach1DecodePositionalCore.cpp libmach1spatial/api_decodepositional/src/Mach1DecodePositionalCAPI.cpp libmach1spatial/api_decodepositional/src/Mach1DecodePositionalEmscripten.cpp \
	-o libmach1spatial/api_decodepositional/include/js/Mach1DecodePositional.js
	emcc -O3 --closure 0 --minify 0 -s MODULARIZE=1 --bind -s ALLOW_TABLE_GROWTH=1 \
	-s "EXPORT_NAME='Mach1EncodeModule'" \
	--pre-js libmach1spatial/api_encode/src/Mach1EncodeEmscripten.js \
	-Ilibmach1spatial/api_common/include -Ilibmach1spatial/api_encode/include \
	libmach1spatial/api_encode/src/Mach1EncodeCore.cpp libmach1spatial/api_encode/src/Mach1EncodeCAPI.cpp libmach1spatial/api_encode/src/Mach1EncodeEmscripten.cpp \
	-o libmach1spatial/api_encode/include/js/Mach1Encode.js
	emcc -O3 --closure 0 --minify 0 -s MODULARIZE=1 --bind -s ALLOW_TABLE_GROWTH=1 \
	-s "EXPORT_NAME='Mach1TranscodeModule'" \
	--pre-js libmach1spatial/api_transcode/src/Mach1TranscodeEmscripten.js \
	-Ilibmach1spatial/api_common/include -Ilibmach1spatial/api_transcode/include -Ilibmach1spatial/api_transcode/matrices -Ilibmach1spatial/deps \
	libmach1spatial/api_transcode/src/Mach1TranscodeCore.cpp libmach1spatial/api_transcode/src/Mach1TranscodeCAPI.cpp libmach1spatial/api_transcode/src/Mach1TranscodeEmscripten.cpp libmach1spatial/api_encode/src/Mach1EncodeCore.cpp libmach1spatial/api_transcode/src/Mach1GenerateCoeffs.cpp libmach1spatial/deps/M1DSP/M1DSPUtilities.cpp libmach1spatial/deps/M1DSP/M1DSPFilters.cpp \
	-o libmach1spatial/api_transcode/include/js/Mach1Transcode.js
