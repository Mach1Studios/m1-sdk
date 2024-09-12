# Mach1Encode API

Mach1Encode allows you to transform input audio streams into the Mach1Spatial VVBP 8 channel format. Included are functions needed for mono, stereo or quad/FOA audio streams. The input streams are referred to as `Points` in our SDK.

The typical encoding process starts with creating an object of a class Mach1EncodeCore, and setting it up as described below. After that, you're meant to generate Points by calling generatePointResults() on the object of this class. You'll get as many points as there are input channels and as many gains in each point as there are output channels. You then copy each input channel to each output channel with the according gain.

## Summary of Use

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
void update(){
    m1Encode.setAzimuth(azimuth);
    m1Encode.setElevation(elevation);
    m1Encode.setDiverge(diverge);
    m1Encode.setAutoOrbit(autoOrbit);
    m1Encode.setStereoRotate(sRotation);
    m1Encode.setStereoSpread(sSpread);
    m1Encode.setPannerMode(Mach1EncodePannerMode::IsotropicLinear);
    m1Encode.setInputMode(Mach1EncodeInputMode::Mono);
    m1Encode.setOutputMode(Mach1EncodeOutputMode::M1Spatial_8);

    m1Encode.generatePointResults();
    std::vector<float> gains = this->gains;
}
```
- <b class="tab-title">Swift</b>
```swift
func update(decodeArray: [Float], decodeMode: Mach1DecodeMode){
    m1Encode.setAzimuth(azimuth: azimuth)
    m1Encode.setElevation(elevation: elevation)
    m1Encode.setDiverge(diverge: diverge)
    m1Encode.setAutoOrbit(setAutoOrbit: true)
    m1Encode.setStereoRotate(setStereoRotation: sRotation)
    m1Encode.setStereoSpread(setStereoSpread: stereoSpread)
    m1Encode.setPannerMode(pannerMode: mode)
    m1Encode.setInputMode(inputMode: mode)
    m1Encode.setOutputMode(outputMode: mode)

    m1Encode.generatePointResults()

    //Use each coeff to decode multichannel Mach1 Spatial mix
    var gains : [Float] = m1Encode.getResultingCoeffsDecoded(decodeMode: decodeMode, decodeResult: decodeArray)

    for i in 0..<players.count {
        players[i].volume = gains[i] * volume
    }
}
```
- <b class="tab-title">JavaScript</b>
```js
    let m1Encode = null;
    Mach1EncodeModule().then(function(m1EncodeModule) {
    m1Encode = new(m1EncodeModule).Mach1Encode();
    });
    function update() {
        m1Encode.setAzimuth(params.azimuth);
        m1Encode.setElevation(params.elevation);
        m1Encode.setDiverge(params.diverge);
        m1Encode.setStereoRotate(params.sRotation);
        m1Encode.setStereoSpread(params.sSpread);
        m1Encode.setAutoOrbit(params.autoOrbit);
        m1Encode.setPannerMode(params.pannerMode);

        m1Encode.generatePointResults();
        var encodeCoeffs = m1Encode.getGains();
    }
```

</div>

The Mach1Encode API is designed to aid in developing tools for inputting to a Mach1 VVBP/SPS format. They give access to common calculations needed for the audio processing and UI/UX handling for panning/encoding Mach1 VVBP/SPS formats via the following common structure:

## Installation

Import and link the appropriate target device's / IDE's library file.

## Generate Point Results

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Encode.generatePointResults();
```
- <b class="tab-title">Swift</b>
```swift
m1Encode.generatePointResults()
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.generatePointResults();
```

</div>

Returns the resulting `points` coefficients based on selected and calculated input/output configuration.

## Set Input Mode

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
if (inputKind == 0) { // Input: MONO
    m1Encode.inputMode = M1Encode::INPUT_MONO;
}
if (inputKind == 1) { // Input: STERO
    m1Encode.inputMode = M1Encode::INPUT_STEREO;
}
if (inputKind == 2) { // Input: Quad
    m1Encode.inputMode = M1Encode::INPUT_QUAD;
}
if (inputKind == 3) { // Input: AFORMAT
    m1Encode.inputMode = M1Encode::INPUT_AFORMAT;
}
if (inputKind == 4) { // Input: BFORMAT
    m1Encode.inputMode = M1Encode::INPUT_FOAACN;
}
```
- <b class="tab-title">Swift</b>
```swift
var mode : Mach1EncodeInputMode = Mach1EncodeInputMode::Mono
m1Encode.setInputMode(inputMode: mode)

if(soundFiles[soundIndex].count == 1) {
    mode = Mach1EncodeInputMode::Mono
}
else if(soundFiles[soundIndex].count == 2) {
    mode = Mach1EncodeInputMode::Stereo
}
else if (soundFiles[soundIndex].count == 4) {
	if (quadMode){
		mode = Mach1EncodeInputMode::Quad
	}
	if (aFormatMode){
		mode = Mach1EncodeInputMode::AFormat
	}
	if (bFormatMode){
		mode = Mach1EncodeInputMode::BFOAACN
	}
}
```
- <b class="tab-title">JavaScript</b>
```js
if (params.inputKind == 0) { // Input: MONO
    m1Encode.setInputMode(m1Encode.Mach1EncodeInputMode.Mono);
}
if (params.inputKind == 1) { // Input: STERO
    m1Encode.setInputMode(m1Encode.Mach1EncodeInputMode.Stereo);
}
if (params.inputKind == 2) { // Input: Quad
    m1Encode.setInputMode(m1Encode.Mach1EncodeInputMode.Quad);
}
if (params.inputKind == 3) { // Input: AFORMAT
    m1Encode.setInputMode(m1Encode.Mach1EncodeInputMode.AFormat);
}
if (params.inputKind == 4) { // Input: 1st Order Ambisonics (ACNSN3D)
    m1Encode.setInputMode(m1Encode.Mach1EncodeInputMode.BFOAACN);
}
```

</div>

Sets the number of input streams to be positioned as points.

 - INPUT_MONO
 - INPUT_STEREO
 - INPUT_QUAD
 - INPUT_LCRS
 - INPUT_AFORMAT
 - INPUT_FOAACN
 - INPUT_FOAFUMA
 - INPUT_2OAACN
 - INPUT_2OAFUMA
 - INPUT_3OAACN
 - INPUT_3OAFUMA
 - INPUT_LCR

## Set Output Mode

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
if (outputKind == 0) { // Output: 4CH Mach1Horizon
    m1Encode.outputMode = M1Encode::M1Spatial_4;
}
if (outputKind == 1) { // Output: 8CH Mach1Spatial
    m1Encode.outputMode = M1Encode::M1Spatial_8;
}
```
- <b class="tab-title">Swift</b>
```swift
if (outputKind == 0) { // Output: 4CH Mach1Horizon
    m1Encode.setOutputMode(outputMode: M1Spatial_4)
}
if (outputKind == 1) { // Output: 8CH Mach1Spatial
    m1Encode.setOutputMode(outputMode: M1Spatial_8)
}
```
- <b class="tab-title">JavaScript</b>
```js
if (params.outputKind == 0) { // Output: 4CH Mach1Horizon
    m1Encode.setOutputMode(m1Encode.mode.M1Spatial_4);
}
if (params.outputKind == 1) { // Output: 8CH Mach1Spatial
    m1Encode.setOutputMode(m1Encode.mode.M1Spatial_8);
}
```

</div>

Sets the output spatial format, Mach1Spatial or Mach1Horizon

 - M1Spatial_4 (4ch) [Yaw]
 - M1Spatial_8 (8ch) [Yaw, Pitch, Roll] {default}
 - M1Spatial_12 (12ch) [Yaw, Pitch, Roll]
 - M1Spatial_14 (14ch) [Yaw, Pitch, Roll]

<!-- ## Set Panner Mode
Sets the style and mode of panner input calculation

 - Mach1EncodePannerModeIsotropicLinear [default]
Acts evenly with distribution across all azimuth/rotation angles and all elevation/pitch angles.

 - Mach1EncodePannerModeIsotropicEqualPower
Acts evenly with distribution across all azimuth/rotation angles and all elevation/pitch angles and applies an equal power multiplier for normalized panning gain

 - Mach1EncodePannerModePeriphonicLinear
Acts evenly with distribution across azimuth/rotation angles but crossfades between the top quadrant and bottom quadrants of output for elevation/pitch angles

```cpp
if (pannerMode == 0) {
    m1Encode.setPannerMode = Mach1EncodePannerMode::IsotropicLinear;
}
if (pannerMode == 1) {
    m1Encode.setPannerMode = Mach1EncodePannerMode::IsotropicEqualPower;
}
if (pannerMode == 2) {
    m1Encode.setPannerMode = Mach1EncodePannerMode::PeriphonicLinear;
}
```

```swift
if (pannerMode == 0) {
    m1Encode.setPannerMode(pannerMode: IsotropicLinear)
}
if (pannerMode == 1) {
    m1Encode.setPannerMode(pannerMode: IsotropicEqualPower)
}
if (pannerMode == 2) {
    m1Encode.setPannerMode(pannerMode: PeriphonicLinear)
}
```

```js
if (params.pannerMode == 0) {
    m1Encode.setPannerMode(m1Encode.Mach1EncodePannerMode.IsotropicLinear);
}
if (params.pannerMode == 1) {
    m1Encode.setPannerMode(m1Encode.Mach1EncodePannerMode.IsotropicEqualPower);
}
if (params.pannerMode == 2) {
    m1Encode.setPannerMode(m1Encode.Mach1EncodePannerMode.PeriphonicLinear);
}
``` -->

## Set Azimuth

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Encode.setAzimuth = azimuthFromMinus1To1;
```
- <b class="tab-title">Swift</b>
```swift
m1Encode.setAzimuth(azimuth: azimuthFromMinus1To1)
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.setAzimuth(params.azimuthFromMinus1To1);
```

</div>

Rotates the point(s) around the center origin of the vector space.
> UI value range: 0.0 -> 1.0 (0 -> 360)

## Set Azimuth Degrees

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Encode.setAzimuthDegrees = azimuthDegrees;
```
- <b class="tab-title">Swift</b>
```swift
m1Encode.setAzimuthDegrees(azimuth: azimuthDegrees)
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.setAzimuthDegrees(params.azimuthDegrees);
```

</div>

Rotates the point(s) around the center origin of the vector space.
> UI value range: 0.0 -> 360.0

## Set Azimuth Radians

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Encode.setAzimuthRadians = azimuthRadians;
```
- <b class="tab-title">Swift</b>
```swift
m1Encode.setAzimuthRadians(azimuth: azimuthRadians)
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.setAzimuthRadians(params.azimuthRadians);
```

</div>

Rotates the point(s) around the center origin of the vector space.
> UI value range: 0 -> 2PI (0 -> 360)

## Set Diverge

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Encode.setDiverge = diverge;
```
- <b class="tab-title">Swift</b>
```swift
m1Encode.setDiverge(diverge: diverge)
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.setDiverge(params.diverge);
```

</div>

Moves the point(s) to/from center origin of the vector space.
> UI value range: -1.0 -> 1.0

## Set Elevation

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Encode.setElevation = elevationFromMinus1to1;
```
- <b class="tab-title">Swift</b>
```swift
m1Encode.setElevation(elevation: elevationFromMinus1to1)
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.setElevation(params.elevationFromMinus1to1);
```

</div>

Moves the point(s) up/down the vector space.
> UI value range: -1.0 -> 1.0 (-90 -> 90)

## Set Elevation Degrees

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Encode.setElevationDegrees = elevationFromMinus90to90;
```
- <b class="tab-title">Swift</b>
```swift
m1Encode.setElevationDegrees(elevation: elevationFromMinus90to90)
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.setElevationDegrees(params.elevationFromMinus90to90);
```

</div>

Moves the point(s) up/down the vector space.
> UI value range: -90 -> 90

## Set Elevation Radians

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Encode.setElevationRadians = elevationFromMinusHalfPItoHalfPI;
```
- <b class="tab-title">Swift</b>
```swift
m1Encode.setElevationRadians(elevation: elevationFromMinusHalfPItoHalfPI)
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.setElevationRadians(params.elevationFromMinusHalfPItoHalfPI);
```

</div>

Moves the point(s) up/down the vector space.
> UI value range: -PI/2 -> PI/2 (-90 -> 90)

## Set Stereo Rotation

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Encode.setStereoRotate = sRotation;
```
- <b class="tab-title">Swift</b>
```swift
m1Encode.setStereoRotate(setStereoRotate: stereoRotate)
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.setStereoRotate(params.sRotation);
```

</div>

Rotates the two stereo points around the axis of the center point between them.
> UI value range: -180.0 -> 180.0

## Set Stereo Spread

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Encode.setStereoSpread = sSpread;
```
- <b class="tab-title">Swift</b>
```swift
m1Encode.setStereoSpread(setStereoSpread: stereoSpread)
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.setStereoSpread(params.sSpread);
```

</div>

Increases or decreases the space between the two stereo points.
> UI value range: 0.0 -> 1.0

## Set Auto Orbit

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
m1Encode.setAutoOrbit = autoOrbit;
```
- <b class="tab-title">Swift</b>
```swift
m1Encode.setAutoOrbit(setAutoOrbit: true)
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.setAutoOrbit(params.autoOrbit);
```

</div>

When active both stereo points rotate in relation to the center point between them so that they always triangulate toward center of the cuboid.
> default value: true

## Inline Mach1Encode Object Decoder

<div class="tabbed">

- <b class="tab-title">C++</b>
```cpp
//Use each coeff to decode multichannel Mach1 Spatial mix
volumes = m1Encode.getResultingCoeffsDecoded(decodeMode, decodeArray)

for (int i = 0; i < 8; i++) {
    players[i].volume = volumes[i] * volume
}
```
- <b class="tab-title">Swift</b>
```swift
//Use each coeff to decode multichannel Mach1 Spatial mix
var volumes : [Float] = m1Encode.getResultingCoeffsDecoded(decodeMode: decodeMode, decodeResult: decodeArray)

for i in 0..<players.count {
    players[i].volume = volumes[i] * volume
}
```
- <b class="tab-title">JavaScript</b>
```js
m1Encode.generatePointResults();

var decoded = m1Decode.decode(params.decoderRotationY, params.decoderRotationP, params.decoderRotationR);

var vol = [];
if (params.outputKind == 1) { // Output: Mach1Spatial
    vol = m1Encode.getResultingCoeffsDecoded(m1Decode.Mach1DecodeMode.M1Spatial_8, decoded);
}
```

</div>

This function allows designs where only previewing or live rendering to decoded audio output is required without any step of rendering or exporting to disk. This enables designs where developers can stack and sum multiple Mach1Encode object's decoded outputs instead of using Mach1Encode objects to write to a master 8 channel intermediary file. Allowing shorthand versions of Mach1Encode->Mach1Decode->Stereo if only live playback is needed.

This can also be used to add object audio design to your application from the Mach1 Spatial APIs and add further control to an application to layer pre-rendered spatial audio and runtime spatial audio as needed.
