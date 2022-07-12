echo "### CLEANING OLD TESTS ###"
rm -rf ./_install

echo "### BUILD macOS ###"
cmake . -B_builds/xcode -GXcode -DCMAKE_INSTALL_PREFIX=`pwd`/_install/xcode
cmake --build _builds/xcode --target install

_install/xcode/bin/Mach1EncodeTests
_install/xcode/bin/Mach1DecodeTests
_install/xcode/bin/Mach1DecodePositionalTests
_install/xcode/bin/Mach1TranscodeTests
_install/xcode/bin/Mach1SpatialTests

echo "### RENDER CHECK ####"
echo "### 5.1_C -> M1Spatial ###"
rm -rf Mach1TranscodeRenderTest/build
cd Mach1TranscodeRenderTest && mkdir build && cd build && cmake ../ && cmake --build . && ./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial -out-file-chans 0