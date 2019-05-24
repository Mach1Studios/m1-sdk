#!/bin/bash

#build all binaries and codesign them for use
if [[ $PWD/ = */m1-sdk/lib/ ]]
then 
	echo $PWD
else
	echo "WHERE ARE YOU!?!?!?!?"
	exit
fi

rm -rf _logs

echo "### BUILD ANDROID ###"
echo "### BUILD NDK11 ###"
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-armeabi
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-armeabi-v7a
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-x86
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-x86-64
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-mips
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-arm64-v8a
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-arm64-v8a
polly --clear --install --config Release --toolchain android-ndk-r11c-api-21-mips64

echo "### BUILD NDK16b ###"
polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-clang-libcxx  
polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-clang-libcxx
polly --clear --install --config Release --toolchain android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx

echo "### BUILD RPI ###"
polly --clear --install --config Release --toolchain raspberrypi2-cxx11
polly --clear --install --config Release --toolchain raspberrypi3-cxx11

echo "Please build release iOS from xcode 8.3.1 only!"
echo "Do you want to build an unsafe for release develop ios lib(s)?"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) 
			echo "### BUILD iOS ###";
			polly --clear --install --config Release --toolchain ios-12-1-dep-9-3 --ios-multiarch --ios-combined; break;;
        No ) break;;
    esac
done

echo "### BUILD macOS ###"
polly --clear --install --config Release --toolchain xcode

#echo "### BUILD cross-compile Linux ###"
#polly --clear --install --config Release --toolchain linux-gcc-x64
#polly --clear --install --config Release --toolchain gcc-static-std

echo "### CODESIGN iOS & macOS ###"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/ios/lib/libMach1DecodeCAPI.a"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/ios/lib/libMach1DecodePositionalCAPI.a"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/ios/lib/libMach1EncodeCAPI.a"

codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1DecodeCAPI.a"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1DecodePositionalCAPI.a"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/lib/libMach1EncodeCAPI.a"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1DecodeCAPI.bundle"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1DecodePositionalCAPI.bundle"
codesign --deep --force --verify --verbose --sign "Developer ID Application: Drazen Bosnjak (6ZETDT84RB)" "/Volumes/git/m1-sdk/lib/_install/xcode/libBundle/libMach1EncodeCAPI.bundle"

echo "### EMSCRIPTEN ASM CONVERSION ###"
emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1DecodeModule'" --pre-js Mach1DecodeEmscripten.js -o external/js/Mach1Decode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp
#emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1DecodePositionalModule'" --pre-js Mach1DecodePositionalEmscripten.js -o external/js/Mach1DecodePositional.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodePositionalCore.cpp Mach1DecodePositionalCAPI.cpp Mach1DecodePositionalEmscripten.cpp -Iinclude/
emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1EncodeModule'" --pre-js Mach1EncodeEmscripten.js -o external/js/Mach1Encode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp

echo "Execute _example_mover_osx.sh?"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) 
			sh ./_example_mover_osx.sh; break;;
        No ) break;;
    esac
done
