REM emcc --bind -s "EXPORT_NAME='Mach1Decode'" -s ALLOW_TABLE_GROWTH="1" --pre-js Mach1DecodeEmscripten.js --post-js Mach1DecodeEmscriptenPost.js -o external/js/Mach1Decode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp

REM emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1DecodeModule'" -s ALLOW_TABLE_GROWTH="1" --pre-js Mach1DecodeEmscripten.js -o external/js/Mach1Decode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp

REM emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1EncodeModule'" -s ALLOW_TABLE_GROWTH="1" --pre-js Mach1EncodeEmscripten.js -o external/js/Mach1Encode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp

REM emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1DecodePositionalModule'" -s ALLOW_TABLE_GROWTH="1" -Iinclude/ --pre-js Mach1DecodePositionalEmscripten.js -o external/js/Mach1DecodePositional.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodePositionalCore.cpp Mach1DecodePositionalCAPI.cpp Mach1DecodePositionalEmscripten.cpp

emcc -O3 --closure 0 --memory-init-file 0 --bind -s MODULARIZE=1 -s "EXPORT_NAME='Mach1TranscodeModule'" -s ALLOW_TABLE_GROWTH="1" --pre-js Mach1TranscodeEmscripten.js -o external/js/Mach1Transcode.js -Iinclude -I../public  Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1TranscodeCore.cpp Mach1TranscodeCAPI.cpp Mach1TranscodeEmscripten.cpp Mach1GenerateCoeffs.cpp include/M1DSP/M1DSPUtilities.cpp include/M1DSP/M1DSPFilters.cpp
