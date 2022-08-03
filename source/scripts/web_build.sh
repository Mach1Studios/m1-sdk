#!/bin/bash

# WASM BUILD on unix
source ${TRAVIS_BUILD_DIR}/emsdk/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind  -s "EXPORT_NAME='Mach1DecodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1DecodeEmscripten.js -o include/js/Mach1Decode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodeEmscripten.cpp
source ${TRAVIS_BUILD_DIR}/emsdk/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind  -s "EXPORT_NAME='Mach1DecodePositionalModule'" -s ALLOW_TABLE_GROWTH=1 -Ideps/ --pre-js Mach1DecodePositionalEmscripten.js -o include/js/Mach1DecodePositional.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1DecodeCore.cpp Mach1DecodeCAPI.cpp Mach1DecodePositionalCore.cpp Mach1DecodePositionalCAPI.cpp Mach1DecodePositionalEmscripten.cpp
source ${TRAVIS_BUILD_DIR}/emsdk/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind  -s "EXPORT_NAME='Mach1EncodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1EncodeEmscripten.js -o include/js/Mach1Encode.js Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1EncodeCore.cpp Mach1EncodeCAPI.cpp Mach1EncodeEmscripten.cpp
source ${TRAVIS_BUILD_DIR}/emsdk/emsdk_env.sh && emcc -O3 --minify 0 --closure 0 -s MODULARIZE=1 --bind  -s "EXPORT_NAME='Mach1TranscodeModule'" -s ALLOW_TABLE_GROWTH=1 --pre-js Mach1TranscodeEmscripten.js -o include/js/Mach1Transcode.js -Iinclude -I../public -Ideps Mach1Point3DCore.cpp Mach1Point4DCore.cpp Mach1TranscodeCore.cpp Mach1TranscodeCAPI.cpp Mach1TranscodeEmscripten.cpp Mach1EncodeCore.cpp Mach1GenerateCoeffs.cpp deps/M1DSP/M1DSPUtilities.cpp deps/M1DSP/M1DSPFilters.cpp
# COPYING FOR WASM
rsync -c include/js/* ../mach1spatial-libs/web
# Upload built libs
aws s3 sync include/js/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/web --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE