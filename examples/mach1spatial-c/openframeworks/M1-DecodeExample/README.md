### Mach1 C++ Decoding Example
#### Description: With debug tools for simulating decoding with variable update rates to orientation.
### 
### Current Audio Formats:
 - Mach1 Spatial / 8channel
 - Mach1 HorizonPairs / 8Channel

#### Build Instructions
 - Download: http://openframeworks.cc/versions/v0.9.8/
 - Download dependencies: 
	- ofxAudioDecoder
	- ofxImGui
	- ofxJSON
	- ofxOsc
- Move ofxMach1 to your addons directory in OpenFrameworks
- Use ProjectGenerator App pointed to repo directory to create Xcode file 
- Add the following lines to your Run Script in Build Phases (this will copy the data folder into the build): 
    ```
    cp -r ./bin/data/ "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"
    
    # Adjust path to your local Openframeworks directory!
    rsync -aved  ../../../../../openFrameworks/addons/ofxMach1/libs/lib/osx/libMach1DecodeCAPI.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Frameworks/";
	install_name_tool -change libMach1DecodeCAPI.dylib @executable_path/../Frameworks/libMach1DecodeCAPI.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
    ```
- After a successful build, copy your 8x mono channels to Resources/[1][2][3]/ for audio playback testing
- Ensure to hardcode the serial port for your controller if using an IMU
