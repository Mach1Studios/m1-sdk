<a href="http://mach1.xyz"><img src="http://mach1.xyz/images/logo_big_b_l.png"></a>


### M1 SDK ###

* Summary
* Included
* Documentation: <a href="http://dev.mach1.xyz">dev.mach1.xyz</a>
* Contact


### Summary ###

*VVBP or Virtual Vector Based Panning is a controlled virtual version of traditional VBAP (Vector Based Amplitude Panning) or SPS (Spatial PCM Sampling). These formats are designed for simplicity and ease of use & implementation both for the content creators and the developers. The spatial audio mixes are based on only amplitude based coefficients changes for both encoding and decoding, and unlike many other spatial audio approaches, there are no additional signal altering processes (such as room modeling, delays or filters) to create coherent and accurate spatial sound fields and play them back from a first person headtracked perspective. Due to the simplicity of the format and cuboid vector space it relies on, it is also ideal for converting and carrying surround and spatial audio mixes without altering the mix to do so, making it an ideal server side audio middleman container. Bringing controlled post-produced spatial audio into new mediums easily.*

#### The Mach1 Spatial SDK includes three components and libraries: ####

* Mach1Encode lib: Encode and process input streams/audio into a Mach1Spatial VVBP format.
* Mach1Decode lib: Decode and process a Mach1Spatial VVBP format with device orientation / headtracking to output directional spatial audio.
* Mach1DecodePositional lib: Add additional optional decoding layer to decode spatial mixes with 6DOF for positional and orientational decoding.
* Mach1Transcode lib: Transcode / convert any audio format (surround/spatial) to or from a Mach1Spatial VVBP format.

*Mach1Encode and Mach1Decode are supported on OSX 10.7+, Windows 10+, iOS 9.0+ and Android API 19+. Unity 4.0+ and Unreal Engine 4.10+ examples are available and said engines are supported too on the aforementioned platforms.*

*Mach1Transcode is supported on OSX and Windows, game engine support coming soon.*


### Included ###

API Binaries built for following Platforms:
* Android r11c - r16b: arm64-v8a
* Android r11c - r16b: armeabi
* Android r11c - r16b: armeabi-v7a
* Android r11c - r16b: mips
* Android r11c - r16b: mips64
* Android r11c - r16b: x86
* Android r11c - r16b: x86-64
* iOS
* Xcode
* Windows-x86 (Visual Studio)
* Windows-x86-64 (Visual Studio)
* Unix
* RaspberryPi2
* RaspberryPi3

Includes:
* C
* C++
* C#
* Java
* js/web
* Swift/Obj-C


Debug Tools:
 
* Simple Audio Debug Examples
* Unity Test Project 

Implementation: 

* Unreal Engine Plugin
* Unity Package
* OpenFrameworks examples in cpp
* iOS example
* Android example
* Web Example

### DOCUMENTATION ###

open or load `docs/index.html` into your preferred browser to run through all the API documentation. 

### Contact ###

Dylan Marcus - 
dylan@mach1studios.com