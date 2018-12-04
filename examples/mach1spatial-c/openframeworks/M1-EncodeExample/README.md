### Mach1 C++ Encoding Demo
#### Description: Uses GUI to illustrate different common methods of encoding to a Mach1 Spatial VVBP8 
### 
### Current Audio Formats:
 - Mach1 Spatial / 8channel
 - Mach1 HorizonPairs / 8Channel

#### Build Instructions
 - Download: http://openframeworks.cc/versions/v0.10.1/
 - Download dependencies: 
	- ofxImGui: https://github.com/jvcleave/ofxImGui
- Move ofxMach1 to your addons directory in OpenFrameworks
- Use ProjectGenerator App pointed to repo directory to create Xcode file 
- Add the following lines to your Run Script in Build Phases (this will copy the data folder into the build): 
    ```
    cp -r ./bin/data/ "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"
    
    # Adjust path to your local Openframeworks directory!
    rsync -aved "$OF_PATH/addons/ofxMach1/libs/lib/osx/libMach1DecodeCAPI.dylib" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Frameworks/";
	install_name_tool -change libMach1DecodeCAPI.dylib @executable_path/../Frameworks/libMach1DecodeCAPI.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
    ```