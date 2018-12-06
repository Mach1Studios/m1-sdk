REM emcc --bind -s "EXPORT_NAME='Mach1Decode'" --pre-js Mach1DecodeEmscripten.js --post-js Mach1DecodeEmscriptenPost.js -o external/js/Mach1Decode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp

emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1DecodeModule'" --pre-js Mach1DecodeEmscripten.js -o external/js/Mach1Decode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp
emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1EncodeModule'" --pre-js Mach1EncodeEmscripten.js -o external/js/Mach1Encode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp

