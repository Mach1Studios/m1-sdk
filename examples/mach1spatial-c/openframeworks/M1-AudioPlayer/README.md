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