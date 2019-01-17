### Mach1 C++ Demonstrations
### 
### 
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
- Add the following lines to your Run Script in Build Phases (this will copy the data folder into the build): 

```
cp -r ./bin/data/ "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"

rsync -aved  "$OF_PATH/addons/ofxMach1/libs/lib/osx/libMach1DecodeCAPI.dylib" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Frameworks/";
install_name_tool -change libMach1DecodeCAPI.dylib @executable_path/../Frameworks/libMach1DecodeCAPI.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";

rsync -aved  "$OF_PATH/addons/ofxMach1/libs/lib/osx/libMach1EncodeCAPI.dylib" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Frameworks/";
install_name_tool -change libMach1EncodeCAPI.dylib @executable_path/../Frameworks/libMach1EncodeCAPI.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
```

- After a successful build, copy your 8x mono channels to Resources/[1][2][3]/ for audio playback testing
- Ensure to hardcode the serial port for your controller if using an IMU
