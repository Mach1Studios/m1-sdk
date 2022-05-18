<a href="http://dev.mach1.tech"><img src="https://mach1-public.s3.amazonaws.com/assets/logo_big_b_l.png"></a>

## MACH1 SPATIAL SDK ##
[![Slack Channel](https://img.shields.io/badge/Slack-Join-purple)](https://join.slack.com/t/spatialaudio/shared_invite/enQtNjk0ODE4NjQ4NjExLWQ5YWUyNWQ4NWEwMDEwZmJiNmI5MzBhYjM3OTE3NTYxYzdjZDE2YTlhZDI4OGY0ZjdkNmM1NzgxNjI5OGU4ZWE)
[![YouTube Channel](https://img.shields.io/badge/YouTube-Subscribe-red)](https://www.youtube.com/channel/UCqoFv8OnTYjkwjHeo6JDUFg)
[![Mach1SpatialAPI CocoaPods](https://cocoapod-badges.herokuapp.com/v/Mach1SpatialAPI/badge.png)](https://www.mach1.tech/developers)
[![Mach1SpatialAPI Jitpack](https://jitpack.io/v/Mach1Studios/JitPack-Mach1SpatialAPI.svg)](https://www.mach1.tech/developers)

* [LICENSE](#license)
* [SUMMARY](#summary)
* [INCLUDED](#included)
* DOCUMENTATION: <a href="http://dev.mach1.tech">dev.mach1.tech</a>
* [CONTACT](#contact)

### [LICENSE](#license) ###

***IMPORTANT NOTICE: PLEASE READ CAREFULLY BEFORE DOWNLOADING, INSTALLING OR USING THE MACH1 SPATIAL SDK:***
 - BY CLICKING ON THE “ACCEPT” BUTTON, OR BY DOWNLOADING, INSTALLING, OR USING THE MACH1 SPATIAL SDK, YOU AGREE TO THE TERMS OF THIS AGREEMENT, AND ALL INCORPORATED TERMS INCLUDING THE PRIVACY POLICY, AS APPLICABLE, WHICH WILL BIND YOU.
- YOU MUST JOIN THE SPATIALAUDIO.SLACK.COM WORKSPACE TO KEEP IN TOUCH ABOUT THIS LICENSE AND MOST RECENT FEATURES ABOUT THE MACH1 SPATIAL SDK.
- The most current version of the MACH1 SPATIAL SDK ROYALTY FREE LICENSE will be posted at www.mach1.tech/license. If the Current Version has a more recent effective date than this document, then this document is replaced by the Currnt Version and by clicking on the appropriate button below, or by downloading, installing, activating or using any of the SDK, or any other update method Mach1 may choose, you agree to be bound by the Current Version. You may not use an old version of any SDK if you have not agreed to the Current Version.
 - IF YOU DO NOT AGREE TO THE TERMS OF THIS AGREEMENT, WE WILL NOT LICENSE THE MACH1 SPATIAL SDK TO YOU AND YOU MUST DISCONTINUE THE INSTALLATION OR DOWNLOAD PROCESS OR CEASE USE OF THE MACH1 SPATIAL SDK.

By downloading and/or using the Mach1 Spatial SDK, including use of any of the contents within the [binaries/](binaries),
you agree to and acknowledge the terms of use set forth by the [Mach1 Spatial SDK License](https://www.mach1.tech/license).
If you do not agree to the terms set forth by the [Mach1 Spatial SDK License](https://www.mach1.tech/license) you are not
permitted to use, link, compile and/or distribute any of the contents of this repository.

Please view the [license/](license) directory for more information and proper attribution resources.

### [SUMMARY](#summary) ###

*Mach1 Spatial VVBP (Virtual Vector Based Panning) is a controlled virtual version of traditional VBAP (Vector Based Amplitude Panning) or SPS (Spatial PCM Sampling). The Mach1 Spatial formats are designed for simplicity and ease of use & implementation both for the content creators and the developers. The Mach1 Spatial audio mixes are based on only amplitude based coefficients changes for both encoding and decoding, and unlike many other spatial audio approaches, there are no additional signal altering processes (such as room modeling, delays or filters) to create coherent and accurate spatial sound fields and play them back from a first person headtracked perspective. Due to the simplicity of the format and vector space it relies on, it is also ideal for converting and carrying surround and spatial audio mixes without altering the mix to do so, making it an ideal server side audio middleman container. Bringing controlled post-produced spatial audio into new mediums easily.*

#### The Mach1 Spatial SDK includes four components and libraries: ####

* Mach1Encode lib: Encode and process input streams/audio into a Mach1Spatial VVBP format.
* Mach1Decode lib: Decode and process a Mach1Spatial VVBP format with device orientation / headtracking to output directional spatial audio.
* Mach1DecodePositional lib: Add additional optional decoding layer to decode spatial mixes with 6DOF for positional and orientational decoding.
* Mach1Transcode lib: Transcode / convert any audio format (surround/spatial) to or from a Mach1Spatial VVBP format.

*Mach1Encode and Mach1Decode are supported on OSX 10.7+, Windows 10+, iOS 9.0+ and Android API 19+. Unity 4.0+ and Unreal Engine 4.10+ examples are available and said engines are supported too on the aforementioned platforms.*

*Mach1Transcode is supported on OSX and Windows, game engine support coming soon.*


### [INCLUDED](#included) ###

#### API Binaries built for following Platforms
* Android (libs in multiple archs, jitpack)
* iOS (cocoapod, lib, frameworks)
* macOS (xcode)
* Linux (x86 & ARM)
* Windows-x86 (Visual Studio)
* Windows-x86-64 (Visual Studio)
* Unix/linux: _m1-transcode executable_
* RaspberryPi2
* RaspberryPi3
* Various embedded ARM chipsets

#### Includes
* C/C++
* C#
* Java
* js/web
* Swift/Obj-C
* Python (in progress)
* Golang (in progress)

#### Debug Tools
* Simple Audio Debug Examples

#### Implementation 
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

### [CONTACT](#contact) ###

General:
info@mach1.tech
