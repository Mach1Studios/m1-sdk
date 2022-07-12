echo "### CLEANING OLD TESTS ###"
rm -rf ./_install

echo "### BUILD Linux ###"
cmake . -B_builds/gcc -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc
cmake --build _builds/gcc --target install

_install/gcc/bin/Mach1EncodeTests
_install/gcc/bin/Mach1DecodeTests
_install/gcc/bin/Mach1DecodePositionalTests
_install/gcc/bin/Mach1TranscodeTests
_install/gcc/bin/Mach1SpatialTests

echo "### RUN LINKING TESTS ON CURRENT DEVICE ###"
g++ -o spatial-decode-exmple -I../include/cpp -L_install/gcc/lib -lpthread -lMach1DecodeCAPI ../examples/mach1spatial-c/commandline/spatial-decode-example/spatial-decode-example/main.cpp 
g++ -o spatial-encode-exmple -I../include/cpp -L_install/gcc/lib -lpthread -lMach1EncodeCAPI ../examples/mach1spatial-c/commandline/spatial-encode-example/spatial-encode-example/main.cpp 
g++ -o spatial-decodepositional-exmple -I../include/cpp -L_install/gcc/lib -lpthread -lMach1DecodeCAPI -lMach1DecodePositionalCAPI ../examples/mach1spatial-c/commandline/spatial-decodepositional-example/spatial-decodepositional-example/main.cpp 
g++ -o spatial-transcode-exmple -I../include/cpp -L_install/gcc/lib -lpthread -lMach1TranscodeCAPI ../examples/mach1spatial-c/commandline/spatial-transcode-example/spatial-transcode-example/main.cpp 

echo "### RENDER CHECK ####"
echo "### 5.1_C -> M1Spatial ###"
rm -rf Mach1TranscodeRenderTest/build
cd Mach1TranscodeRenderTest && mkdir build && cd build && cmake ../ && cmake --build . && ./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial -out-file-chans 0