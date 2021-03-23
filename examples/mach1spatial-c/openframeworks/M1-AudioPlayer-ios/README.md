### Mach1 C++ Additional Audio Example
### 
### 
### Current Audio Formats:
 - Mach1 Spatial / 8channel
 - Mach1 HorizonPairs / 8Channel

#### Build Instructions
 - Download: http://openframeworks.cc/versions/v0.10.1/
 - Download dependencies: 
	- ofxImGui: https://github.com/jvcleave/ofxImGui
	- ofxJSON
	- ofxOsc
- Move ofxMach1 to your addons directory in OpenFrameworks
- Use ProjectGenerator App pointed to repo directory to create Xcode file 
- After a successful build, copy your 8x mono channels to Resources/[1][2][3]/ for audio playback testing
- Ensure to hardcode the serial port for your controller if using an IMU