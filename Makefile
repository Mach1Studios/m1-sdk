# Mach1 Spatial SDK

# getting OS type
ifeq ($(OS),Windows_NT)
	detected_OS := Windows
else
	detected_OS := $(shell uname)
endif

pull:
	git pull --recurse-submodules

git-nuke:
	git submodule foreach --recursive git clean -x -f -d
	make pull

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
	brew install cmake emscripten pre-commit libsndfile rtaudio doxygen llvm
	sudo ln -s "$(brew --prefix llvm)/bin/clang-format" "/usr/local/bin/clang-format"
	sudo ln -s "$(brew --prefix llvm)/bin/clang-tidy" "/usr/local/bin/clang-tidy"
	sudo ln -s "$(brew --prefix llvm)/bin/clang-apply-replacements" "/usr/local/bin/clang-apply-replacements"
	pre-commit install
else
	sudo apt-add-repository universe && sudo apt-get update && sudo apt install libsndfile-dev cmake emscripten librtaudio-dev pre-commit doxygen
	pre-commit install
endif

.PHONY: clean
clean:
ifeq ($(detected_OS),Windows)
	@if exist _builds (rmdir /s /q _builds)
else
	rm -rf _builds
endif

doxygen:
	cd docs && doxygen Doxyfile

SOURCES := $(shell find libmach1spatial/api_*/** -name '*.cpp')
HEADERS := $(shell find libmach1spatial/api_*/** -name '*.h')
SOURCES += $(HEADERS)

# Filter out any files that have "emscripten" in their name
SOURCES := $(filter-out %Emscripten%, $(SOURCES))

CLANG_TIDY='clang-tidy'

.PHONY: lint
lint:
	command -v ${CLANG_TIDY} >/dev/null 2>&1 || { echo >&2 "I require ${CLANG_TIDY} but it's not installed.  Aborting."; exit 1; }
	@for FILE in $(SOURCES) ; do \
		echo "Running clang-tidy on $$FILE with config:$(CURDIR)/libmach1spatial/.clang-tidy..." ; \
		clang-tidy -fix-errors "$$FILE" --config-file=$(CURDIR)/libmach1spatial/.clang-tidy -- -I$(HEADERS) ; \
	done
	@echo "Done: libmach1spatial styled and tidied"

.PHONY: check-style
check-style:
	@for FILE in $(SOURCES) ; do \
		var=`clang-format "$$FILE" | diff "$$FILE" - | wc -l` ; \
		if [ $$var -ne 0 ] ; then \
			echo "$$FILE does not respect the coding style (diff: $$var lines)" ; \
			exit 1 ; \
		fi ; \
		var2=`clang-tidy "$$FILE" | diff "$$FILE" - | wc -l` ; \
		if [ $$var2 -ne 0 ] ; then \
			echo "$$FILE does not respect the coding style (diff: $$var2 lines)" ; \
			exit 1 ; \
		fi ; \
	done
	@echo "Passed: libmach1spatial checks pass"

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

# Add a new target to update JitPack-Mach1SpatialAPI with the latest libmach1spatial code
update-jitpack:
	@echo "Updating JitPack-Mach1SpatialAPI with the latest libmach1spatial code..."
	# Update common header files
	rsync -c libmach1spatial/api_common/include/*.h examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/
	# Update decode headers and sources
	rsync -c libmach1spatial/api_decode/include/*.h examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/
	rsync -c libmach1spatial/api_decode/src/*.cpp examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/src/ --exclude='*Emscripten*'
	rsync -c libmach1spatial/api_decode/src/*.h examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/src/
	# Update decode positional headers and sources
	rsync -c libmach1spatial/api_decodepositional/include/*.h examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/
	rsync -c libmach1spatial/api_decodepositional/src/*.cpp examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/src/ --exclude='*Emscripten*'
	rsync -c libmach1spatial/api_decodepositional/src/*.h examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/src/
	# Update encode headers and sources
	rsync -c libmach1spatial/api_encode/include/*.h examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/
	rsync -c libmach1spatial/api_encode/src/*.cpp examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/src/ --exclude='*Emscripten*'
	rsync -c libmach1spatial/api_encode/src/*.h examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/src/
	# Update transcode headers and sources
	rsync -c libmach1spatial/api_transcode/include/*.h examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/include/
	rsync -c libmach1spatial/api_transcode/src/*.cpp examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/src/ --exclude='*Emscripten*'
	rsync -c libmach1spatial/api_transcode/src/*.h examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/src/
	# Update JNI wrapper files
	rsync -c libmach1spatial/swig/jni/java/com/mach1/spatiallibs/* examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/java/com/mach1/spatiallibs/
	rsync -c libmach1spatial/swig/jni/*.cxx examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/Mach1SpatialLibs/src/main/cpp/mach1/
	@echo "JitPack-Mach1SpatialAPI updated successfully!"

# Also modify deploy-android to run update-jitpack first
deploy-android: clean generate-jni-wrapper update-jitpack
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
	@echo "Starting macOS build..."
	cmake . -B_builds/osx \
	-GXcode \
	-DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=MAC_UNIVERSAL
	cmake --build _builds/osx --config Release
	cmake --install _builds/osx --config Release
	@echo "Starting iOS build..."
	cmake . -B_builds/ios \
	-GXcode \
	-DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=OS64COMBINED
	cmake --build _builds/ios --config Release # separate build and install steps for fat-lib
	cmake --install _builds/ios --config Release
	@echo "Starting tvOS build..."
	cmake . -B_builds/tvos \
	-GXcode \
	-DM1S_BUILD_TESTS=OFF -DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=TVOSCOMBINED
	cmake --build _builds/tvos --config Release # separate build and install steps for fat-lib
	cmake --install _builds/tvos --config Release
	@echo "Starting watchOS build..."
	cmake . -B_builds/watchos \
	-GXcode \
	-DM1S_BUILD_TESTS=OFF -DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=WATCHOSCOMBINED
	cmake --build _builds/watchos --config Release # separate build and install steps for fat-lib
	cmake --install _builds/watchos --config Release
	@echo "Starting visionOS build..."
	cmake . -B_builds/xros \
	-GXcode \
	-DM1S_BUILD_TESTS=OFF -DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=VISIONOS
	cmake --build _builds/xros --config Release # separate build and install steps for fat-lib
	cmake --install _builds/xros --config Release
	@echo "Starting visionSimulator build..."
	cmake . -B_builds/xrsimulator \
	-GXcode \
	-DM1S_BUILD_TESTS=OFF -DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=SIMULATOR_VISIONOS
	cmake --build _builds/xrsimulator --config Release # separate build and install steps for fat-lib
	cmake --install _builds/xrsimulator --config Release
	@echo "Delete the previous xcframework files..."
	rm -rf examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/libMach1DecodeCAPI.xcframework
	rm -rf examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/libMach1DecodePositionalCAPI.xcframework
	rm -rf examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/libMach1EncodeCAPI.xcframework
	rm -rf examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/libMach1TranscodeCAPI.xcframework
	xcodebuild -create-xcframework -library examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/device/libMach1DecodeCAPI.a -library examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/simulator/libMach1DecodeCAPI.a -output examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/libMach1DecodeCAPI.xcframework
	xcodebuild -create-xcframework -library examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/device/libMach1DecodePositionalCAPI.a -library examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/simulator/libMach1DecodePositionalCAPI.a -output examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/libMach1DecodePositionalCAPI.xcframework
	xcodebuild -create-xcframework -library examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/device/libMach1EncodeCAPI.a -library examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/simulator/libMach1EncodeCAPI.a -output examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/libMach1EncodeCAPI.xcframework
	xcodebuild -create-xcframework -library examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/device/libMach1TranscodeCAPI.a -library examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/simulator/libMach1TranscodeCAPI.a -output examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/Mach1SpatialAPI/Lib/xros/libMach1TranscodeCAPI.xcframework
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

# Define variables
WIN64_SRC_DIR = _builds/windows-x86_64/libmach1spatial/Release
WIN64_DEST_DIR = examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/Windows/x86_64
WIN32_SRC_DIR = _builds/windows-x86/libmach1spatial/Release
WIN32_DEST_DIR = examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/Windows/x86

deploy-unity: clean
ifeq ($(detected_OS),Darwin)
	@echo "Making folders if they do not exist..."
	@mkdir -p examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/iOS
	@mkdir -p examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/visionOS/Device
	@mkdir -p examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/visionOS/Simulator
	@mkdir -p examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS
	@echo "Starting macOS Unity build..."
	@echo "deleting the old bundle directories for cmake to nicely reinstall"
	rm -rf examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle
	rm -rf examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/libMach1DecodePositionalCAPI.bundle
	rm -rf examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/libMach1EncodeCAPI.bundle
	rm -rf examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/libMach1TranscodeCAPI.bundle
	cmake . -B_builds/macos -GXcode -DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_UNITY_LIBS=ON -DCMAKE_BUILD_TYPE=Release
	cmake --build _builds/macos --config Release --target install
	mv examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/Mach1DecodeCAPI.bundle examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/libMach1DecodeCAPI.bundle
	mv examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/Mach1DecodePositionalCAPI.bundle examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/libMach1DecodePositionalCAPI.bundle
	mv examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/Mach1EncodeCAPI.bundle examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/libMach1EncodeCAPI.bundle
	mv examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/Mach1TranscodeCAPI.bundle examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/macOS/libMach1TranscodeCAPI.bundle
	@echo "Starting iOS Unity build..."
	mkdir -p examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/iOS
	cmake . -B_builds/ios \
	-GXcode \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_UNITY_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=OS64COMBINED
	cmake --build _builds/ios --config Release # separate build and install steps for fat-lib
	cmake --install _builds/ios --config Release
	#@echo "Starting tvOS Unity build..."
	#cmake . -B_builds/tvos \
	#-GXcode \
	#-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_UNITY_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	#-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=TVOSCOMBINED
	#cmake --build _builds/tvos --config Release
	#cmake --build _builds/tvos --config Release --target install
	@echo "Starting visionOS Unity build..."
	cmake . -B_builds/visionos \
	-GXcode \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_UNITY_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=VISIONOS #TODO: make fat lib
	cmake --build _builds/visionos --config Release
	cmake --install _builds/visionos --config Release
	@echo "Starting visionOS Simulator Unity build..."
	cmake . -B_builds/visionsimulator \
	-GXcode \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_UNITY_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=SIMULATOR_VISIONOS
	cmake --build _builds/visionsimulator --config Release 
	cmake --install _builds/visionsimulator --config Release
endif
ifeq ($(detected_OS),Windows)
	@echo "Starting Windows x86_64 Unity build..."
	cmake . -B_builds/windows-x86_64 -A x64 \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_UNITY_LIBS=ON -DCMAKE_BUILD_TYPE=Release
	cmake --build _builds/windows-x86_64 --config Release --target install
	@echo "Copying DLLs from $(WIN64_SRC_DIR) to $(WIN64_DEST_DIR)"
	@mkdir $(WIN64_DEST_DIR) 2>nul || echo "Folder already exists"
	@copy /Y "$(WIN64_SRC_DIR)\*.dll" "$(WIN64_DEST_DIR)"
	@echo "Starting Windows x86 Unity build..."
	cmake . -B_builds/windows-x86 -A Win32 \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_UNITY_LIBS=ON -DCMAKE_BUILD_TYPE=Release
	cmake --build _builds/windows-x86 --config Release --target install
	@echo "Copying DLLs from $(WIN32_SRC_DIR) to $(WIN32_DEST_DIR)"
	@mkdir $(WIN32_DEST_DIR) 2>nul || echo "Folder already exists"
	@copy /Y "$(WIN32_SRC_DIR)\*.dll" "$(WIN32_DEST_DIR)"
endif
	@echo "Starting Android arm64 Unity build..."
	cmake . -B_builds/android-arm64-v8a \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_UNITY_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake \
	-DCMAKE_SYSTEM_NAME=Android \
	-DANDROID_PLATFORM=21 \
	-DANDROID_ABI=arm64-v8a \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DCMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK}
	cmake --build _builds/android-arm64-v8a --config Release --target install
	@echo "Starting Android armeabi-v7a Unity build..."
	cmake . -B_builds/android-armeabi-v7a \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_UNITY_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake \
	-DCMAKE_SYSTEM_NAME=Android \
	-DANDROID_PLATFORM=21 \
	-DANDROID_ABI=armeabi-v7a \
	-DCMAKE_ANDROID_ARM_NEON=ON \
	-DCMAKE_ANDROID_ARM_MODE=ON \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DCMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK}
	cmake --build _builds/android-armeabi-v7a --config Release --target install
	@echo "Starting Android x86 Unity build..."
	cmake . -B_builds/android-x86 \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_UNITY_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake \
	-DCMAKE_SYSTEM_NAME=Android \
	-DANDROID_PLATFORM=21 \
	-DANDROID_ABI=x86 \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DCMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK}
	cmake --build _builds/android-x86 --config Release --target install
	@echo "Starting Android x64 Unity build..."
	cmake . -B_builds/android-x86-64 \
	-DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_TESTS=OFF -DBUILD_UNITY_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake \
	-DCMAKE_SYSTEM_NAME=Android \
	-DANDROID_PLATFORM=21 \
	-DANDROID_ABI=x86_64 \
	-DCMAKE_ANDROID_STL_TYPE=c++_static \
	-DCMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK}
	cmake --build _builds/android-x86-64 --config Release --target install
	@echo "Copy all .cs files..."
	rsync -c --include='*.cs' --include='*/' --exclude='*' libmach1spatial/ examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1

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
