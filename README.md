<a href="http://dev.mach1.tech"><img src="https://mach1-public.s3.amazonaws.com/assets/logo_big_b_l.png"></a>

## MACH1 SPATIAL SDK ##
[![libmach1spatial Version](https://img.shields.io/badge/libmach1spatial-v4.0-blue)](https://www.mach1.tech/developers)
[![Test](https://github.com/Mach1Studios/m1-sdk-dev/actions/workflows/test-sdk.yaml/badge.svg)](https://github.com/Mach1Studios/m1-sdk-dev/actions/workflows/test-sdk.yaml)
[![YouTube Channel](https://img.shields.io/badge/YouTube-Subscribe-red)](https://www.youtube.com/channel/UCqoFv8OnTYjkwjHeo6JDUFg)
[![Mach1SpatialAPI CocoaPods](https://img.shields.io/cocoapods/v/Mach1SpatialAPI.svg?style=flat)](https://www.mach1.tech/developers)
[![Mach1SpatialAPI Jitpack](https://jitpack.io/v/Mach1Studios/JitPack-Mach1SpatialAPI.svg)](https://www.mach1.tech/developers)

* [LICENSE](#license)
* [SUMMARY](#summary)
* [INCLUDED](#included)
* DOCUMENTATION: <a href="http://dev.mach1.tech">dev.mach1.tech</a>
* [CONTACT](#contact)

### [LICENSE](#license) ###

By downloading and/or using the Mach1 Spatial SDK, including use of any of the contents within the [libmach1spatial/](libmach1spatial/),
you agree to and acknowledge the terms of use set forth by the [Mach1 Spatial SDK License](LICENSE.txt).
If you do not agree to the terms set forth by the [Mach1 Spatial SDK License](LICENSE.txt) you are not
permitted to use, link, compile and/or distribute any of the contents of this repository.

Please view the [license/](license) directory for more information and proper attribution resources.

### [SUMMARY](#summary) ###

*Mach1 Spatial VVBP (Virtual Vector Based Panning) is a virtual VBAP (Vector Based Amplitude Panning) or SPS (Spatial PCM Sampling) framework of multichannel symmetrical layouts designed for inter-operability with other multichannel audio approaches and packaged in flexible APIs for use.*
*The Mach1 Spatial layouts are designed for simplicity and ease of use & implementation both for the content creators and the developers. The Mach1 Spatial audio mixes are based on only amplitude based coefficients changes for both encoding and decoding, and unlike many other spatial audio approaches, there are no additional signal altering processes (such as room modeling, delays or filters) to create coherent and accurate spatial sound fields and play them back from a first person headtracked perspective. Due to the simplicity of the framework and vector space it relies on, it is also ideal for converting and carrying surround and spatial audio mixes without altering the mix to do so, making it an ideal server side audio middleman container and bringing controlled post-produced spatial audio into new mediums more easily.*

#### The Mach1 Spatial SDK includes four components and libraries: ####

* Mach1Encode: Encode and process input streams/audio into a Mach1Spatial VVBP format.
* Mach1Decode: Decode and process a Mach1Spatial VVBP format with device orientation / headtracking to output directional spatial audio.
* Mach1DecodePositional: Add additional optional decoding layer to decode spatial mixes with 6DOF for positional and orientational decoding.
* Mach1Transcode: Transcode / convert any audio format (surround/spatial) to or from a Mach1Spatial VVBP format.

### [SETUP](#setup) ###

#### CMake
The easiest way to use this project is to use CMake from the root directory to setup all the APIs, examples and tests as needed. An example of this is running the following from this directory:
- `cmake . -B build -DM1S_BUILD_TESTS=ON -DM1S_BUILD_EXAMPLES=ON -DM1ENCODE_INLINE_DECODE=ON -DM1TRANSCODE_INLINE_ENCODE=ON -DM1S_BUILD_SIGNAL_SUITE=ON`
- `cmake --build build --config Release`

View the [CMakeLists.txt](CMakeLists.txt) file to see any options, by default the examples and tests are enabled.

##### Options
- `M1ENCODE_INLINE_DECODE`: Includes Mach1Decode API into Mach1Encode API as a dependency to allow more features.
- `M1TRANSCODE_INLINE_ENCODE`: Includes Mach1Encode API into Mach1Transcode API as a dependency to allow more features.

#### Makefile
The makefile also has quick commands for setting up and building the entire SDK
- `make test`
- `make build`

### [INSTALL](#install) ###

#### CMake
Linking or including any of the APIs to a project can also be easily done via cmake, and the APIs are individually modular in case you want to limit which of them is needed.

##### Rebuild Cocoapods libs
- `cmake . -B_builds/ios -GXcode \
  -DM1S_BUILD_EXAMPLES=OFF -DBUILD_COCOAPODS_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE=libmach1spatial/cmake/ios-cmake/ios.toolchain.cmake -DPLATFORM=OS64COMBINED`
- `cmake --build _builds/ios --config Release # separate build and install steps for fat-lib`
- `cmake --install _builds/ios --config Release`

##### Rebuild Android libs
Make sure you have setup or preinstalled your Android NDK and set the following environment var: `CMAKE_ANDROID_NDK`
- `cmake . -B_builds/android-arm64-v8a \
  -DM1S_BUILD_EXAMPLES=OFF -DBUILD_JITPACK_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake \
  -DCMAKE_SYSTEM_NAME=Android \
  -DANDROID_PLATFORM=21 \
  -DANDROID_ABI=arm64-v8a \
  -DCMAKE_ANDROID_STL_TYPE=c++_static \
  -DCMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK}`
- `cmake --build _builds/android-arm64-v8a --config Release --target install`

#### Makefile
Call any of these premade make commands from this directory to quickly recompile any libs as needed.

##### Rebuild WASM/JS Web libs
- `make deploy-web`

##### Rebuild Cocoapods libs
- `make deploy-ios`

##### Rebuild Android libs
- `make deploy-android`


#### Implementation Examples
* Unreal Engine Plugin <a href="https://www.mach1.tech/download-spatialgameengine">Unreal Engine Plugin</a>
	* _UE Plugin for Mach1Decode/Mach1DecodePositional_
* Unity Package <a href="https://www.mach1.tech/download-spatialgameengine">Unity Package</a>
	* _.unitypackage for Mach1Decode/Mach1DecodePositional and experiemental DSPGraph implementation of Mach1Encode_
* iOS Examples
	* [_Spatial Audio Player example_](https://github.com/Mach1Studios/Pod-Mach1SpatialAPI/tree/master/Examples/spatialaudioplayer-example)
	* _Mach1Decode example_
	* _Mach1Transcode example_
	* _Mach1Encode & live Mach1Decode example_
	* _Mach1DecodePositional example_
* Android Examples
	* _Mach1Decode example_
	* _Mach1Encode & live Mach1Decode example_
* Web Examples
	* _Mach1Decode example_
	* [_Facetracking WebPlayer example_](https://github.com/Mach1Studios/m1-web-spatialaudioplayer)
	* _Mach1Encode & live Mach1Decode example_
	* _Mach1Encode Recorder example_
* OpenFrameworks Examples in C++
	* _Mach1Decode example_
	* _Mach1Encode example_
	* _Mach1Decode audio player example_
* CommandLine Examples
	* [_Mach1Decode example_](https://github.com/Mach1Studios/m1-sdk/tree/master/examples/mach1spatial-c/commandline/spatial-decode-example)
	* [_Mach1Transcode example_](https://github.com/Mach1Studios/m1-sdk/tree/master/examples/mach1spatial-c/commandline/spatial-transcode-example)
	* [_Mach1Transcode Audioplayer example_](https://github.com/Mach1Studios/m1-sdk/tree/master/examples/mach1spatial-c/commandline/spatial-transcode-audioplayer)
	* [_Mach1Transcode Render example_](https://github.com/Mach1Studios/m1-sdk/tree/master/examples/mach1spatial-c/commandline/spatial-transcode-render-example)
	* [_Mach1Transcode ObjectAudio example_](https://github.com/Mach1Studios/m1-sdk/tree/master/examples/mach1spatial-c/commandline/spatial-transcode-objectaudio-example)
	* [_Mach1DecodePositional example_](https://github.com/Mach1Studios/m1-sdk/tree/master/examples/mach1spatial-c/commandline/spatial-decodepositional-example)
	* [_Mach1Encode example_](https://github.com/Mach1Studios/m1-sdk/tree/master/examples/mach1spatial-c/commandline/spatial-encode-example)

### DOCUMENTATION ###

open or load [`docs/index.html`](https://github.com/Mach1Studios/m1-sdk/tree/master/docs) into your preferred browser to run through all the API documentation or visit our [hosted documentation](https://dev.mach1.tech)

#### Migration ####

Migration guides from older versions of the Mach1 Spatial SDK can be found in [MIGRATIONS.md](MIGRATIONS.md).

### [CONTACT](#contact) ###

General:
whatsup@mach1.tech
