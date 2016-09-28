### Mach1 C++ Demonstration Env
### 
### 
### Current Audio Formats:
 - M1 Square [Pair] / 4Pair
 - M1 Square [Multichannel] / 4Channel
 - M1 Cube [Multichannel] / 8Channel
### Experimental Audio Formats: 
 - M1 Triangle [Multichannel] / 3Channel
 - M1 Triangular Prism [Multichannel] / 6Channel

#### Build Instructions
 - Download: http://openframeworks.cc/versions/v0.9.3/of_v0.9.3_osx_release.zip
 - Download dependencies: 
   - https://github.com/jefftimesten/ofxJSON
   - https://github.com/jvcleave/ofxImGui
- Use ProjectGenerator App pointed to repo directory to create Xcode file 
- Add the following lines to your Run Script in Build Phases (this will copy the data folder into the build): 
    - `cp -r ./bin/data/ "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"`
    - `cp -r ./bin/data/*.png "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources"`
- Ensure to hardcode the serial port for your controller if using an IMU
