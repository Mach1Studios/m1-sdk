
* On OS X, you must have this in the Run Script Build Phase of your Xcode project. where the first path ../../../addons/ofxMach1/ is the path to the ofxMach1 addon.

```
rsync -aved  "$OF_PATH/addons/ofxMach1/libs/lib/osx/libMach1DecodeCAPI.dylib" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Frameworks/";
install_name_tool -change libMach1DecodeCAPI.dylib @executable_path/../Frameworks/libMach1DecodeCAPI.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
```

If you don't have this you'll see an error in the console: dyld: Library not loaded: libMach1DecodeCAPI.dylib