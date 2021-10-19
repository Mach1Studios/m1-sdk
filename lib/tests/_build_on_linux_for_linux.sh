echo "### BUILD Linux ###"
echo "### REMINDER: Set alias for polly ###"
~/git/polly/bin/polly --clear --install --config Release --toolchain gcc

echo "### RUN TESTS ###"
_install/gcc/bin/Mach1EncodeTest
_install/gcc/bin/Mach1DecodeTest
_install/gcc/bin/Mach1DecodePositionalTest

echo "### RUN LINKING TESTS ON CURRENT DEVICE ###"
cd ../ && ~/git/polly/bin/polly --clear --install --config Release --toolchain gcc
g++ -o spatial-decode-exmple -I../include/cpp -L_install/gcc/lib -lpthread -lMach1DecodeCAPI ../examples/mach1spatial-c/commandline/spatial-decode-example/spatial-decode-example/main.cpp 
g++ -o spatial-encode-exmple -I../include/cpp -L_install/gcc/lib -lpthread -lMach1EncodeCAPI ../examples/mach1spatial-c/commandline/spatial-encode-example/spatial-encode-example/main.cpp 
g++ -o spatial-decodepositional-exmple -I../include/cpp -L_install/gcc/lib -lpthread -lMach1DecodeCAPI -lMach1DecodePositionalCAPI ../examples/mach1spatial-c/commandline/spatial-decodepositional-example/spatial-decodepositional-example/main.cpp 
g++ -o spatial-transcode-exmple -I../include/cpp -L_install/gcc/lib -lpthread -lMach1TranscodeCAPI ../examples/mach1spatial-c/commandline/spatial-transcode-example/spatial-transcode-example/main.cpp 