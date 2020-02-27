# Mach1 Spatial DSPGraph Unity Package

## Description
An example of Mach1SpatialAPIs (Mach1EncodeCAPI, Mach1DecodeCAPI, Mach1DecodePositionalCAPI) using Unity's DSPGraph Nodes to allow users to record to disk Mach1 Spatial mixes for cross platform audio use and spatial audio capturing pipelines. 

## Disclaimer
This implementation is just an example of use of Mach1 Spatial libraries, it may require more finessing and scripting to work as needed depending on current platform and versioning of dependencies. We will continue to support development on this however it is strictly made available as is. 

## Usage
The `M1SpatialDecode.cs` component now contains a new mode selection `Source Type`. Selecting `Audio Clip (Assets)` or `External Audio (Streaming Assets)` will revert `M1SpatialDecode.cs` back to its original performance of a simple Mach1DecodePositional object per mix.
`Encode Object` turns the object into a psuedo `AudioListener` for a single Mach1 Spatial render, by attaching `M1BaseEncode.cs` to each conventional `AudioSource` with the same settings will let you capture it within a Mach1 Spatial render fro mthe `Encode Object`. Please further play with `heightDistanceMax` and `divergeDistanceMax` to fine tune the vector space and capturing as needed.

##### Contact: info@mach1.tech
###### Copyright: Mach1 2020