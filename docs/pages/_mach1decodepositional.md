# Mach1DecodePositional API

Mach1DecodePositional allows the 3DOF orientation decoding to decode in a dev environment that supports 6DOF with positional movement. It does this by referencing the user's device to a location and adding an additional layer of rotations and attenuations to the spatial decoding.

## Unity & Unreal Engine

Please view the examples in `examples/Unity|UnrealEngine` to see deployment of Mach1Spatial mixes with positional rotation and attenuation applied. These fucntions can be viewed from the M1Base class used in both examples and are called by creating a new object in the game engine and attaching Mach1SpatialActor or Mach1SpatialDecode.cs to view the setup for a Mach1 Spatial mix layer.

<aside class="notice">These implementations require the multichannel mixes to be split into multiple-mono mixes. You can use `m1-transcode` or the M1-Transcoder GUI application to achieve this easily.</aside>

## Summary Use
```cpp
void setup(){
    mach1DecodePositional.setDecodeAlgoType(Mach1DecodeAlgoSpatial_8);
    mach1DecodePositional.setPlatformType(Mach1PlatformDefault);
    mach1DecodePositional.setUseAttenuation(bool useAttenuation);
    mach1DecodePositional.setAttenuationCurve(float attenuationCurve);
    mach1DecodePositional.setUsePlaneCalculation(bool usePlaneCalculation);
}

void loop(){
    mach1DecodePositional.setListenerPosition(Mach1Point3D devicePos);
    mach1DecodePositional.setListenerRotation(Mach1Point3D deviceRot);
    mach1DecodePositional.setDecoderAlgoPosition(Mach1Point3D objPos);
    mach1DecodePositional.setDecoderAlgoRotation(Mach1Point3D objRot);
    mach1DecodePositional.setDecoderAlgoScale(Mach1Point3D objScale);

    mach1DecodePositional.getDist();
    mach1DecodePositional.getCoefficients(float* result);
}
```

```swift
override func viewDidLoad() {
    mach1DecodePositional.setDecodeAlgoType(newAlgorithmType: Mach1DecodeAlgoSpatial_8)
    mach1DecodePositional.setPlatformType(type: Mach1PlatformiOS)
    mach1DecodePositional.setFilterSpeed(filterSpeed: 1.0)
    mach1DecodePositional.setUseAttenuation(useAttenuation: true)
    mach1DecodePositional.setUsePlaneCalculation(bool: false)
}
func update() {
    mach1DecodePositional.setListenerPosition(point: (devicePos))
    mach1DecodePositional.setListenerRotation(point: Mach1Point3D(deviceRot))
    mach1DecodePositional.setDecoderAlgoPosition(point: (objectPosition))
    mach1DecodePositional.setDecoderAlgoRotation(point: Mach1Point3D(objectRotation))
    mach1DecodePositional.setDecoderAlgoScale(point: Mach1Point3D(x: 0.1, y: 0.1, z: 0.1))

    mach1DecodePositional.evaluatePositionResults()

    var attenuation : Float = mach1DecodePositional.getDist()
    attenuation = mapFloat(value: attenuation, inMin: 0, inMax: 3, outMin: 1, outMax: 0)
    attenuation = clampFloat(value: attenuation, min: 0, max: 3)
    mach1DecodePositional.setAttenuationCurve(attenuationCurve: attenuation)

    var decodeArray: [Float] = Array(repeating: 0.0, count: 18)
    mach1DecodePositional.getCoefficients(result: &decodeArray)
}
```

The Mach1DecodePositional API is designed to be added if 6DOF or positional placement of Mach1Decode objects are needed. Once added and used for updating the object's and referencable device/camera it will calculate the positional and rotational angles and distances and result them via the same useable coefficients that are used for Mach1Decode, as per the following way:

Setup Step (setup/start):

 - `setDecodeAlgoType`
 - `setPlatformType`
 - `setUseAttenuation` set distance attenuation for soundfield
 - `setAttenuationCurve` design custom distance attenuation curves
 - `setUsePlaneCalculation` reference rotations use a plane instead of a point, or closest plane of a shape if needed

Audio Loop:

 - update device/camera position & rotation (can use Euler or Quat)
 - update m1obj decode position & rotation (can use Euler or Quat)
 - `getDist` used for attenuation/falloff results
 - `getCoefficients` resulting coeffs for players

## Installation

Import and link the appropriate target device's / IDE's library file.

For Unity:
 - Import the Custom Asset Package

For Unreal Engine:
 - Add the Mach1Spatial Plugin to your project

<aside class="notice">Requires libMach1DecodeCAPI for reference to spatial object and orientation decoding</aside>

## Setup per Spatial Soundfield Position

The following are functions to aid in how positional distance effects the overall gain of an mach1decode object to any design. The resulting distance calculations can also be used for any external effect if created.

### Attenuation/Falloff
```cpp
void setUseAttenuation(bool useAttenuation);
```

```swift
func setUseAttenuation(useAttenuation: Bool)
```

Boolean turning on/off distance attenuation calculations on that mach1decode object
<aside class="notice">setUseFalloff has been deprecated</aside>

### Reference positional point/plane/shape
```cpp
void setUsePlaneCalculation(bool usePlaneCalculation);
```

```swift
func setUsePlaneCalculation(bool usePlaneCalculation: Bool)
```

This very long named function can set whether the rotational pivots of a mach1decode soundfield are by referencing the device/camera to a positional point or the closest point of a plane (and further the closest plane of a shape). This allows each mach1decode object to be placed with more design options to prevent soundfield scenes from rotating when not needed.

### Set Filter Speed
```cpp
float filterSpeed = 1.0f;

mach1Decode.setFilterSpeed(filterSpeed);
```

```swift
mach1Decode.setFilterSpeed(filterSpeed: 1.0)
```

Filter speed determines the amount of angle smoothing applied to the orientation angles used for the Mach1DecodeCore class. 1.0 would mean that there is no filtering applied, 0.1 would add a long ramp effect of intermediary angles between each angle sample. It should be noted that you will not have any negative effects with >0.9 but could get some orientation latency when <0.85. The reason you might want angle smoothing is that it might help remove a zipper effect seen on some poorer performing platforms or devices.

## Setup for Advanced Settings

### Mute Controls
```cpp
void setMuteWhenOutsideObject(bool muteWhenOutsideObject);
```

```swift
func setMuteWhenOutsideObject(muteWhenOutsideObject: Bool)
```

Similar to the `setUseClosestPointRotationMuteInside` these functions give further control over placing a soundfield positionally and determining when it should/shouldn't output results.

```cpp
void setMuteWhenInsideObject(bool muteWhenInsideObject);
```

```swift
func setMuteWhenInsideObject(muteWhenInsideObject: Bool)
```

Mute mach1decode object (all coefficifient results becomes 0) when outside the positional reference shape/point

Mute mach1decode object (all coefficifient results becomes 0) when inside the positional reference shape/point

### Manipulate input angles for positional rotations
```cpp
void setUseYawForRotation(bool useYawForRotation);
```

```swift
func setUseYawForRotation(bool useYawForRotation: Bool)
```

Ignore Yaw angle rotation results from pivoting positionally

```cpp
void setUsePitchForRotation(bool usePitchForRotation);
```

```swift
func setUsePitchForRotation(bool usePitchForRotation: Bool)
```

Ignore Pitch angle rotation results from pivoting positionally

```cpp
void setUseRollForRotation(bool useRollForRotation);
```

```swift
func setUseRollForRotation(bool useRollForRotation: Bool)
```

Ignore Roll angle rotation results from pivoting positionally

## Update per Spatial Soundfield Position
Updatable variables for each mach1decode object. These are also able to be set once if needed.

```cpp
void setListenerPosition(Mach1Point3DCore* pos);
```

```swift
func setListenerPosition(point: Mach1Point3D)
```

Updates the device/camera's position in desired x,y,z space
<aside class="notice">setCameraPosition has been deprecated</aside>

```cpp
void setListenerRotation(Mach1Point3DCore* euler);
```

```swift
func setListenerRotation(point: Mach1Point3D)
```

Updates the device/camera's orientation with Euler angles (yaw, pitch, roll)
<aside class="notice">setCameraRotation has been deprecated</aside>

```cpp
void setListenerRotationQuat(Mach1Point4DCore* quat);
```

```swift
func setListenerRotationQuat(point: Mach1Point4D)
```

Updates the device/camera's orientation with Quaternion
<aside class="notice">setCameraRotationQuat has been deprecated</aside>

```cpp
void setDecoderAlgoPosition(Mach1Point3DCore* pos);
```

```swift
func setDecoderAlgoPosition(point: Mach1Point3D)
```

Updates the decode object's position in desired x,y,z space

```cpp
void setDecoderAlgoRotation(Mach1Point3DCore* euler);
```

```swift
func setDecoderAlgoRotation(point: Mach1Point3D)
```

Updates the decode object's orientation with Euler angles (yaw, pitch, roll)

```cpp
void setDecoderAlgoRotationQuat(Mach1Point4DCore* quat);
```

```swift
func setDecoderAlgoRotationQuat(point: Mach1Point4D)
```

Updates the decode object's orientation with Quaternion

```cpp
void setDecoderAlgoScale(Mach1Point3DCore* scale);
```

```swift
func setDecoderAlgoScale(point: Mach1Point3D)
```

Updates the decode object's scale in desired x,y,z space

## Applying Resulting Coefficients
```cpp
void evaluatePositionResults();
```

```swift
func evaluatePositionResults()
```

Calculate!

```cpp
void getCoefficients(float *result);
```

```swift
func getCoefficients(result: inout [Float])
```

Get coefficient results for applying to mach1decode object for rotational and positional, replaces the results from: `mach1Decode.decode`

### Return Relative Comparisons

#### Distance
```cpp
float getDist();
```

```swift
func getDist() -> Float
```

Get normalized distance between mach1decode object and device/camera

#### Current Angle
```cpp
Mach1Point3D getCurrentAngle();
```

```swift
func getCurrentAngle() -> Mach1Point3D
```

Get the current angle of the mach1decode object

#### Current Rotation
```cpp
Mach1Point3D getCoefficientsRotation();
```

```swift
func getCoefficientsRotation() -> Mach1Point3D
```
Get the current rotation of the mach1decode object

### Update Falloff/Attenuation
```cpp
void setAttenuationCurve(float attenuationCurve);
```

```swift
func setAttenuationCurve(attenuationCurve: Float)
```

Set a resulting float of that curve to the current buffer
