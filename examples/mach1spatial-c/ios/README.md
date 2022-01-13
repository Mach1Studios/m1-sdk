### Mach1 iOS Examples

### Current Demos:
 - m1-example: Encoding example with every API installed and a simple GUI example included
 - m1-decode-example: Decoding example with variable device conditions and update rates 
 - m1-trasncode-example: Transcode & Decode example combined into one, example includes a first order ambisonic & 5.1 mix that is re-rendered to Mach1 Spatial and played back with Mach1Decode
 - m1-positional-example: Decoding example with Mach1DecodePositional API also installed

## Installation
- Change directory to one of the examples within the cocoapods directory: `cd Pod-Mach1SpatialAPI/Examples/mach1-example`
- `pod install`
- open the .xcworkspace and build

## Manual Library Installation
- Check that the parent directory of `libMach1DecodeCAPI.a` to your Library Search Paths to import Mach1Decode lib.
Example: `"$(SRCROOT)/../../../../../../mach1spatial-libs/ios/lib/"` 

## Setup
Ideally you will want to decide on your preffered way to stream audio to the Mach1Decode lib depending on whether you application supports
multichannel audio files or not. 