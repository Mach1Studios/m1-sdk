# Mach1Transcode Commandline

Mach1Transcode includes functions for use cases that utilizing Mach1Spatial's agnostic abilities and allows 1:1 VBAP style conversions from any surround or spatial audio format and to any other surround or spatial audio format. This is very helpful for apps that have certain input requirements but different output requirements based on whether the app is launched for VR/AR/MR or just mobile use without completely redesigning the application's structure for audio. This is also a recommended method of carrying one master spatial audio container and at endpoints converting it as needed without adverse signal altering effects seen in other spatial audio formats.

## Usage

Rapidly offline render to and from Mach1 formats.

> Example in command line for converting Mach1Spatial mix to First Order ambisonics: ACNSN3D

```shell
m1-transcode -in-file /path/to/file.wav -in-fmt M1Spatial-8 -out-fmt ACNSN3D -out-file /path/to/output.wav -out-file-chans 0
```

> Example in command line for converting 7.1 film mix to Mach1Spatial

```shell
m1-transcode -in-file /path/to/file.wav -in-fmt 7.1_C -out-fmt Mach1Spatial-8 -out-file /path/to/output.wav
```

> Example in command line for converting Mach1Spatial to Mach1HorizonPairs (quad-binaural compliant)

```shell
m1-transcode -in-file /path/to/file.wav -in-fmt M1Spatial-8 -out-fmt Mach1HorizonPairs -out-file /path/to/output.wav -out-file-chans 2
```

## Suggested Metadata Spec [optional]

> Mach1Spatial-8 = `mach1spatial-8`

> Mach1Spatial-12= `mach1spatial-12`

> Mach1Spatial-14 = `mach1spatial-16`

> Mach1 StSP = `mach1stsp-2`

> Mach1Spatial-4 = `mach1horizon-4`

> Mach1Horizon Pairs = `mach1horizon-8`

Metadata is not required for decoding any Mach1 Spatial VVBP format, and often it is not recommended to rely on auto-detection methods but instead rely on UI/UX for user input upon uploading a Mach1 multichannel audio file for safest handling. This is due to their being several possible 8 channel formats and unless there are proper methods to filter and detect and handle each one, user input will be a safer option. There are many oppurtunities for transcoding or splitting a multichannel audio file all of which could undo metadata or apply false-positive metadata due to the many audio engines not built to handle multichannel solutions safely.

If autodetection is still required, use the following suggested specifications which will be applied to mixes that run out of M1-Transcoder and soon m1-transcode directly:

@note Implemented via ICMT/comment of common codec tags

> Example:

```shell
  Metadata:
    comment         : mach1spatial-8
```

## Examples of Metadata Spec
ffmpeg (wav output): `-metadata ICMT="mach1spatial-8"`

ffmpeg (vorbis output): `-metadata spatial-audio='mach1spatial-8'`

ffmpeg (aac output): `-metadata comment='mach1spatial-8'`

libsndfile (wav output): `outfiles[i].setString(0x05, "mach1spatial-8");`

## Formats Supported
The most up to date location for the supported formats are: [Supported Formats List](https://github.com/Mach1Studios/m1-sdk/blob/master/public/Mach1TranscodeConstants.h#L98-L243)

### [Mach1 & Vector Based Formats](https://github.com/Mach1Studios/m1-sdk/blob/master/public/Mach1TranscodeVectorFormats.h)

### [Surround Formats](https://github.com/Mach1Studios/m1-sdk/blob/master/public/Mach1TranscodeSurroundFormats.h)

### [Ambisonic & Spherical Formats](https://github.com/Mach1Studios/m1-sdk/blob/master/public/Mach1TranscodeAmbisonicFormats.h) (special thanks to [VVAudio](https://www.vvaudio.com/))

### [Mic Array Formats](https://github.com/Mach1Studios/m1-sdk/blob/master/public/Mach1TranscodeMicArrayFormats.h)

@note Additional formats available upon request.

### Custom Format/Configuration

```shell
./m1-transcode -in-file /path/to/16channel.wav -in-fmt CustomPoints -in-json /path/to/16ChannelDescription.json -out-file /path/to/output-m1spatial.wav -out-fmt M1Spatial-8 -out-file-chans 0
```

Input JSON description of the surround/spatial soundfield setup per your design and input it with the `-in-json` arguement for any custom input or output transcoding.

To use this set the `-in-fmt` or `-out-fmt` as `CustomPoints`

## Additional Features

### LFE/SUB Channel Filter

> Example of low pass filtering every channel but the Front-Right of the Mach1 Spatial mix and outputting it to stereo.

```shell
./m1-transcode -in-file /path/to/input-m1spatial.wav -in-fmt M1Spatial-8 -out-file /path/to/output-stereo.wav -lfe-sub 0,2,3,4,6,7 -out-fmt Stereo -out-file-chans 0
```

Use `-lfe-sub` arguement to indicate which input channels you want to apply a Low Pass Filter to, the arguement exapects a list of ints with commas to separate them.

### Spatial Downmixer

```shell
./m1-transcode -in-file /path/to/input-fiveOne.wav -in-fmt 5.1_C -spatial-downmixer 0.9 -out-file /path/to/output-m1spatial.wav -out-fmt M1Spatial-8 -out-file-chans 0
```

For scaling audio outputting to streaming use cases of Mach1Decode and use cases using the Mach1 Spatial output from Mach1Transcode we have included a way to compare the top vs. bottom of the input soundfield, if the difference is less than the set threshold (float) output format will be Mach1 Horizon. This is to allow soundfields that do not have much of a top vs bottom difference to output to a lesser channel Mach1 Horizon format to save on filesize while streaming.

`-spatial-downmixer` arguement can be used to set this, the float after this arguement will be used as the threshold. If used this will effectively add an additional transoding after anything outputting to `Mach1 Spatial` to then transcode from `Mach1 Spatial` to `Mach1 Horizon` while respecting the content of the soundfield.

### Metadata Extractor

```shell
./m1-transcode -in-file /path/to/input-ADM.wav -in-fmt 7.1.4_C_SIM -out-file /path/to/output.wav -extract-metadata -out-fmt M1Spatial-8 -out-file-chans 0
```

An ADM metadata reader and parser is embedded into m1-transcode binary executable to help with custom pipelines using Mach1Encode API to render Object Audio Soundfields into Mach1 Spatial mixes/renders for easier handling.

`-extract-metadata` will dump any found XML ADM metadata in the audio binary as a textfile with the same output name and path.

# Mach1Transcode API

Mach1Transcode leverages the benefits of the Mach1 Spatial virtual vector based panning (VVBP) priniciples to enable faster than real time multichannel audio format conversions safely to retain the soundfield and not use any additional processing effects for simulation. Lack of processing effects ensures transparent input and output soundfields during conversion and the lightweight modular design of the API allows use with any audio handler or media library already natively installed.

Multichannel audio development and creative use currently has a lot of challenges plagued by legacy surround implementations, the Mach1Transcode API can be used to help customize multichannel and spatial audio pipelines in development and garner control without requiring adoption of legacy practices.

## Summary of Use

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
static void* decode(void* v);
Mach1Transcode m1Transcode;
static std::vector<std::vector<float>> m1Coeffs; //2D array, [input channel][input channel's coeff]
Mach1TranscodeFormatType inputMode;
Mach1TranscodeFormatType outputMode;

// Mach1 Transcode Setup
inputMode = "ACNSN3DmaxRE3oa";
outputMode = "M1Spatial-8";

//resize coeffs array to the size of the current output
m1Transcode.setOutputFormat(outputMode);
for (int i = 0; i < m1Coeffs.size(); i++){
    m1Coeffs[i].resize(m1Transcode.getOutputNumChannels(), 0.0f);
}

m1Transcode.setInputFormat(inputMode);
m1Transcode.setOutputFormat(outputMode);

// Called to update Mach1Transcode
m1Transcode.setSpatialDownmixer();
m1Transcode.processConversionPath();
m1Coeffs = m1Transcode.getMatrixConversion();
```
- <b class="tab-title">Swift</b>
```swift
import Mach1SpatialAPI
private var m1Decode = Mach1Decode()
private var m1Transcode = Mach1Transcode()

// Mach1 Transcode Setup
m1Transcode.setInputFormat(inFmt: Mach1TranscodeFormatType)
m1Transcode.setOutputFormat(outFmt: "M1Spatial-8")
m1Transcode.processConversionPath()
matrix = m1Transcode.getMatrixConversion()
// Mach1 Decode Setup
m1Decode.setPlatformType(type: Mach1PlatformiOS)
m1Decode.setDecodeMode(newAlgorithmType: M1Spatial_8)
m1Decode.setFilterSpeed(filterSpeed: 1.0)

// Called when updating InputFormat for Mach1Transcode
m1Decode.beginBuffer()
m1Decode.setRotationDegrees(newRotationDegrees: Mach1Point3D(x: 0, y: 0, z: 0))
let result: [Float] = m1Decode.decodeCoeffsUsingTranscodeMatrix(matrix: matrix, channels: m1Transcode.getInputNumChannels())
m1Decode.endBuffer()

// Called when updating input orientation for Mach1Decode
m1Decode.beginBuffer()
m1Decode.setRotationDegrees(newRotationDegrees: Mach1Point3D(x: Float(deviceYaw), y: Float(devicePitch), z: Float(deviceRoll)))
let result: [Float] = m1Decode.decodeCoeffsUsingTranscodeMatrix(matrix: matrix, channels: m1Transcode.getInputNumChannels())
m1Decode.endBuffer()
```

</div>

The Mach1Transcode API is designed openly by supplying a coefficient matrix for conversion, intepreted as needed.
However, the following will be an example of setting up Mach1Transcode for any input and for direct conversion to Mach1Spatial to be decoded with orientation to stereo for spatial previewing applications:

## Installation

Import and link the appropriate target device's / IDE's library file and headers.

## Set / Get Input Format

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
Mach1TranscodeFormatType inputMode;
m1Transcode.setInputFormat(inputMode);
```
- <b class="tab-title">Swift</b>
```swift
m1Transcode.setInputFormat(inFmt: Mach1TranscodeFormatFiveOneFilm_Cinema)
```

</div>

Set or return the input format/configuration for processing.

## Set / Get Output Format

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
Mach1TranscodeFormatType outputMode;
m1Transcode.setOutputFormat(outputMode);
```
- <b class="tab-title">Swift</b>
```swift
m1Transcode.setOutputFormat(outFmt: Mach1TranscodeFormatM1Spatial)
```

</div>

Set or return the output format/configuration for processing.

## Set / Get Spatial Downmixer

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Transcode.setSpatialDownmixer();
```
- <b class="tab-title">Swift</b>
```swift
m1Transcode.setSpatialDownmixer()
```

</div>

Sets the threshold float for `getSpatialDownmixerPossibility` calculation. The `getSpatialDownmixerPossibility` returns true if the compared signals are less than the `setSpatialDownmixer(corrThreshold)`.

> Float from 0.0 to 1.0 where 0.0 no difference and incrementing to 1.0 is more difference

> When returned true; transcodings that are set to ouput to `Mach1Spatial` will process an additional conversion to `Mach1Horizon`

## Set LFE / Sub Channels

Applys a low pass filter (LPF) to each indicated channel index of the input format and soundfield. Input vector of ints representing the index of input channels to be processed.

## Set Input as Custom Points

Sets the input format for transcoding from an external JSON source. View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions.

## Set Input as ADM

Sets the input format for transcoding from the parsed ADM metadata within the audiofile.

## Process Master Gain

Applys an input gain to the output soundfield.

> Parameters: Input buffer, Integer of input number of samples, Float for gain multiplier

## Process Conversion Path

Use this function to control when to call for calculating the format transcoding calculations.

## Get Conversion Path

Returns the shortest found conversion path to get from input format X to output format Y, both set by `Mach1Transcode::setInputFormat(Mach1TranscodeFormatType inFmt)` and `Mach1Transcode::setOutputFormat(Mach1TranscodeFormatType outFmt)`. Majority of format instances will use Mach1Spatial as the middle format for non-Mach1-format -> non-Mach1-format transcodings. This is due to Mach1 Spatial being a platonic solid format, ideal for safe calculations without loss

## Process Conversion Matrix

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
std::vector<std::vector<float>> m1Coeffs; //2D array, [input channel][input channel's coeff]
m1Coeffs = m1Transcode.getMatrixConversion();
```
- <b class="tab-title">Swift</b>
```swift
private var matrix: [[Float]] = []
matrix = m1Transcode.getMatrixConversion()
```

</div>

Returns the transcoding matrix of coefficients based on the set input and output formats.

## Process Conversion

```swift
m1Transcode.processConversion(float: inBufs, float: outBufs, int: numSamples)
```

Call to process the conversion as set by previous functions.

## Direct Agnostic Playback of All Input Formats via Mach1Decode

<div class="tabbed">

- <b class="tab-title">C++</b>
```swift
// Basic struct for input audio/format
struct AudioInput {
    var name: String
    var format: Mach1TranscodeFormatType
    var files: [String]
}

// Declarations
private var m1Decode = Mach1Decode()
private var m1Transcode = Mach1Transcode()
private var players: [AVAudioPlayer] = []
private var matrix: [[Float]] = []

// Setup
m1Transcode.setInputFormat(inFmt: AudioInput.format)
m1Transcode.setOutputFormat(outFmt: Mach1TranscodeFormatM1Spatial)
m1Transcode.processConversionPath()
matrix = m1Transcode.getMatrixConversion()

// Loop
m1Decode.beginBuffer()
m1Decode.setRotationDegrees(newRotationDegrees: Mach1Point3D(x: 0, y: 0, z: 0)) // Update orientation as needed
let result: [Float] = m1Decode.decodeCoeffsUsingTranscodeMatrix(matrix: matrix, channels: m1Transcode.getInputNumChannels())
m1Decode.endBuffer()

//Use each coeff to decode the multichannel Mach1 Spatial
for i in 0..<result.count {
    players[i].setVolume(result[i], fadeDuration: 0)
}
```

</div>
