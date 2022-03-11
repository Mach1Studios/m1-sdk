#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source/tests

echo "WIN: TESTS"
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain vs-15-2017

echo "RUN POLLY TESTS"
echo " " | _install/vs-15-2017/bin/Mach1EncodeTests.exe
echo " " | _install/vs-15-2017/bin/Mach1DecodeTests.exe
echo " " | _install/vs-15-2017/bin/Mach1DecodePositionalTests.exe
echo " " | _install/vs-15-2017/bin/Mach1SpatialTests.exe
echo " " | _install/vs-15-2017/bin/Mach1TranscodeTests.exe

#TODO: fix windows render testing (path find sndfile)
#echo "RENDER CHECK#"
#echo "5.1_C -> M1Spatial"
#cd Mach1TranscodeRenderTest
#mkdir -p build && cd build
#cmake ../
#cmake --build .
#./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial -out-file-chans 0