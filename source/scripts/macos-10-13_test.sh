#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source/tests

echo "MACOS: TESTS"
cmake . -B_builds/osx-10-13 -GXcode -DCMAKE_INSTALL_PREFIX=`pwd`/_install/osx-10-13
cmake --build _builds/osx-10-13 --config Release --target install

echo "RUN TESTS"
_install/osx-10-13/bin/Mach1EncodeTests
_install/osx-10-13/bin/Mach1DecodeTests
_install/osx-10-13/bin/Mach1DecodePositionalTests
_install/osx-10-13/bin/Mach1SpatialTests
_install/osx-10-13/bin/Mach1TranscodeTests

echo "RENDER CHECK#"
echo "5.1_C -> M1Spatial"
cd Mach1TranscodeRenderTest
mkdir -p build && cd build
cmake ../
cmake --build .
#TODO: Fix bug preventing render test from running
# ./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial -out-file-chans 0