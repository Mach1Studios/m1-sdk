#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source/tests

echo "MACOS: TESTS"
cmake . -B_builds/xcode -GXcode -DCMAKE_INSTALL_PREFIX=`pwd`/_install/xcode
cmake --build _builds/xcode --config Release --target install

echo "RUN POLLY TESTS"
_install/xcode/bin/Mach1EncodeTests
_install/xcode/bin/Mach1DecodeTests
_install/xcode/bin/Mach1DecodePositionalTests
_install/xcode/bin/Mach1SpatialTests
_install/xcode/bin/Mach1TranscodeTests

echo "RENDER CHECK#"
echo "5.1_C -> M1Spatial"
cd Mach1TranscodeRenderTest
wget -O https://github.com/Mach1Studios/m1-sdk-dev/raw/feature/prep-ci/source/tests/Mach1TranscodeRenderTest/m1-debug-shortpt-fiveone.wav
mkdir -p build && cd build
cmake ../
cmake --build .
#TODO: Fix bug preventing render test from running
#./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial -out-file-chans 0