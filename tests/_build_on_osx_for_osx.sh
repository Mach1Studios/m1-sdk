echo "### CLEANING OLD TESTS ###"
rm -rf ./_install
rm -rf ./_builds

echo "### BUILD macOS ###"
cmake . -B_builds/xcode -GXcode -DCMAKE_INSTALL_PREFIX=`pwd`/_install/xcode
cmake --build _builds/xcode --target install

_install/xcode/bin/Mach1EncodeTests || { echo 'Mach1Encode API test failed...' ; exit 1; }
_install/xcode/bin/Mach1DecodeTests || { echo 'Mach1Decode API test failed...' ; exit 1; }
_install/xcode/bin/Mach1DecodePositionalTests || { echo 'Mach1DecodePositional API test failed...' ; exit 1; }
_install/xcode/bin/Mach1TranscodeTests || { echo 'Mach1Transcode API test failed...' ; exit 1; }
_install/xcode/bin/Mach1SpatialTests || { echo 'Additional API tests failed...' ; exit 1; }

echo "### RENDER CHECK ####"
echo "### 5.1_C -> M1Spatial ###"
rm -rf Mach1TranscodeRenderTest/build
cd Mach1TranscodeRenderTest && mkdir build && cd build && cmake ../ && cmake --build . && ./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial-8 -out-file-chans 0 || { echo 'Render test failed...' ; exit 1; }
echo "### FINISHED TESTING ###"