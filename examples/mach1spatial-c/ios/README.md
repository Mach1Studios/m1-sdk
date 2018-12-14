## Setup
This example uses AudioKit to playback multiple channels of a streamed audio file.
Ideally you will want to decide on your preffered way to stream audio to the Mach1Decode lib depending on whether you application supports
multichannel audio files or not. 

- Please download AudioKit and install the compiled framework in the same directory as the project: `mach1-ios-example/AudioKit-iOS`
`https://audiokit.io/downloads/`

- Import `AudioKit.framework` into the Solution Browser of your project in Xcode.

- Add the parent directory of `libMach1DecodeCAPI.a` to your Library Search Paths to import Mach1Decode lib.
Example: `"$(SRCROOT)/../../../../binaries/ios/lib/"` 

