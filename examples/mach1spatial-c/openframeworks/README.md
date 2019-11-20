### Mach1 C++ OpenFrameworks Examples

### Current Demos:
 - M1-Decode Example: Decoding example with variable device conditions and update rates 
 - M1-Encode Example: Encoding example with visual GUI
 - M1-AudioPlayer: Additional Decoding example

#### Build Instructions
 - Download: http://openframeworks.cc/versions/v0.10.1/
 - Download dependencies (varies per demo): 
	- ofxAudioDecoder: https://github.com/kylemcdonald/ofxAudioDecoder
	- ofxImGui: https://github.com/jvcleave/ofxImGui
- Move ofxMach1 to your addons directory in OpenFrameworks
- Use ProjectGenerator App pointed to repo directory to create Xcode file 
- After a successful build, copy your 8x mono channels to Resources/[1][2][3]/ for audio playback testing
- Ensure to hardcode the serial port for your controller if using an IMU
