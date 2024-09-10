# Mach1 SDK Reference & Documentation

# Introduction

Mach1 Spatial SDK includes APIs to allow developers to design applications that can encode or pan to a spatial audio render from audio streams and/or playback and decode Mach1Spatial 8channel spatial audio mixes with orientation to decode the correct stereo output sum of the user's current orientation. Additionally the Mach1 Spatial SDK allows users to safely convert surround/spatial audio mixes to and from the Mach1Spatial or Mach1Horizon **VVBP** formats.

**VVBP** or *Virtual Vector Based Panning* is a controlled virtual version of traditional **VBAP** (*Vector Based Amplitude Panning*) or **SPS** (*Spatial PCM Sampling*). These formats are designed for simplicity and ease of use & implementation both for the content creators and the developers. The spatial audio mixes are based on only amplitude based coefficients changes for both encoding and decoding, and unlike many other spatial audio approaches, there are no additional signal altering processes (such as room modeling, delays or filters) to create coherent and accurate spatial sound fields and play them back from a first person headtracked perspective. Due to the simplicity of the format and cuboid vector space it relies on, it is also ideal for converting and carrying surround and spatial audio mixes without altering the mix to do so, making it an ideal server side audio middleman container. Bringing controlled post-produced spatial audio into new mediums easily.

# Overview

The Mach1 Spatial SDK includes the following components and libraries:

  - ***Mach1Encode lib:***
    Encode and process input streams/audio into a Mach1Spatial VVBP format.
  - ***Mach1Decode lib:***
    Decode and process a Mach1Spatial VVBP format with device orientation / headtracking to output directional spatial audio.
  - ***Mach1DecodePositional lib:***
    Add additional optional decoding layer to decode spatial mixes with 6DOF for positional and orientational decoding.
  - ***Mach1Transcode lib:***
    Transcode / convert any audio format (surround/spatial) to or from a Mach1Spatial VVBP format.

# Mach1 Internal Angle Standard
We decided to cherrypick and clarify how we think and describe rotations & translations in space, focused more to unify creators and developers and derived more from a first person perspective. After long deliberation on using various existing standards, they had places where they worked and places where they didn’t, they also were not very “humanized” and in an effort to fix this, we follow these guidelines:

### Coordinate / Angle / Rotation Description Expectations:
   - Rotations can be individually explained per axis with signed rotations
   - Rotations are explained from a center perspective point of view (FPV - First Person View)

### Mach1 YPR Polar Expectation of Describing Orientation:
_Common use: Mach1Decode API, Mach1DecodePositional API_

   - Yaw (left -> right | where rotating left is negative)
   - Pitch (down -> up | where rotating down is negative)
   - Roll (top-pointing-left -> top-pointing-right | where rotating top of object left is negative)

### Mach1 AED Expectation of Describing Polar Points:
_Common use: Mach1Encode API_

   - Azimuth (left -> right | where rotating left is negative)
   - Elevation (down -> up | where rotating down is negative)
   - Diverge (backward -> forward | where behind origin of Azimuth/Elevation is negative)

### Mach1 XYZ Coordinate Expectation of Vector Points:
   - X (left -> right | where -X is left)
   - Y (front -> back | where -Y is back)
   - Z (top -> bottom | where -Z is bottom)

### Positional 3D Coords
  -  X+ = strafe right
  -  X- = strafe left
  -  Y+ = up
  -  Y- = down
  -  Z+ = forward
  -  Z- = backward

### Orientation Euler
  -  Yaw[0]+ = rotate right [Range: 0->360 | -180->180]
  -  Yaw[0]- = rotate left [Range: 0->360 | -180->180]
  -  Pitch[1]+ = rotate up [Range: -90->90]
  -  Pitch[1]- = rotate down [Range: -90->90]
  -  Roll[2]+ = tilt right [Range: -90->90]
  -  Roll[2]- = tilt left [Range: -90->90]
_The orientation convention is based on the first person perspective point of view to make interfacing as easy to interpret as possible._

# JSON Descriptions
When utilizing the `CustomPoints` formats for the Mach1Transcode API for either the input or output format description you can describe the custom format via JSON using the following syntax and example:

### Concept
Each input channel is described as a "point", this can be described either spherically via `usePolar` or via cartesian coordinates either of which should match the descriptions outlined in [Mach1 Internal Angle Standard](#mach1-internal-angle-standard).

#### Advanced
  - [IN DEVELOPMENT] `gain` descriptions can be added for further custom normalization schemes.

### Point Description

#### Using Cartesian
  - `x` with values between -1.0 and 1.0
  - `y` with values between -1.0 and 1.0
  - `z` with values between -1.0 and 1.0

#### Using Polar / Spherical (in degrees)
  - `azimuth` with values between -180.0 and 180.0 _(conversions to radians happen internally)_
  - `elevation` with values between -180.0 and 180.0 _(conversions to radians happen internally)_
  - `diverge` with values between -1.0 and 1.0

  - `name` a string description of the channel or point
  - `usePolar` a boolean description of skipping cartesian input of the point, expected as `0`/`1` or `false`/`true` _(REQUIRED FOR POLAR/SPHERICAL DESCRIPTIONS OF EACH POINT)_

### Example
The following is a 2 channel example description
```cpp
 {
    "points": [
        {
            "x": 0.0,
            "y": 0.0,
            "z": 0.0,
            "usePolar": true,
            "azimuth": -45.0,
            "elevation": 0.0,
            "diverge": 1.0,
            "name": "L"
        },
        {
            "x": 1.0,
            "y": 1.0,
            "z": 0.0,
            "name": "R"
        },
    ]
}
```

# Common Implementation Issues
The following is a list of common heard issues during implementation and include audio tools to help find these issues as well as basic descriptions of their behavior and how they can be avoided.

### Orientation Latency Issues
@note
Orientation Latency is usually the biggest concern in the current climate of devices that need some sort of headtracking or window tracking. This is an issue for visuals but just as much for audio as well, if the angles inputted into the Mach1Decode functions are late then so are the returned coefficients which leads to the user losing immersion and effect.

### Orientation Rate Issues (Zipper)
@note
If the rate of angles sending to the app layer with Mach1Decode integrated is too slow, this can cause some audible distortion / clicking issues added to the mix. The effect will sound like a zipper as you headtrack faster and faster. This is due to the difference of the returned coefficients being too large from result to result, causing the gain change applied to be too large and creating a "jump". We have included ways to filter the input angles into our functions however this is a band-aid to the issue if your target device has this issue. The idea is to filter and delay the inputting angles adding more intermidiary angles between each "jump" but at the cost of Orientation Latency (see above).

### Audio/Visual Sync Issues
@note
It is important to ensure sync between a visual element in your project / application and the spatial audio mix. Ensure they are managed by the same function receiving the same call.

### Spatial Decoding Phase Issues
@note
If you have your spatial audio streams coming from different audio managers that do not have sync between them you will start hearing strange channel phasing issues while you track your head orientation or device orientation. Eliminate these by ensuring your audio is called at the same time and managed properly.
