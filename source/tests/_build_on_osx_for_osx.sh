echo "### BUILD macOS ###"
mkdir build && cd build
cmake .. -G Xcode
cmake --build .

echo "### RENDER CHECK ####"
echo "### 5.1_C -> M1Spatial ###"
rm -rf Mach1TranscodeRenderTest/build
cd Mach1TranscodeRenderTest && mkdir build && cd build && cmake ../ && cmake --build . && ./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial -out-file-chans 0