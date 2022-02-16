echo "### BUILD macOS ###"
polly --clear --install --config Release --toolchain xcode

echo "### RUN TESTS ###"
_install/xcode/bin/Mach1EncodeTest
_install/xcode/bin/Mach1DecodeTest
_install/xcode/bin/Mach1DecodePositionalTest

echo "### RENDER CHECK ####"
echo "### 5.1_C -> M1Spatial ###"
cd Mach1TranscodeRenderTest && mkdir build && cd build && cmake ../ && cmake --build . && ./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial -out-file-chans 0