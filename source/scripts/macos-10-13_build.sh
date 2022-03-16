#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source
# MACOS 10-13 BUILDS
${POLLY_SOURCE_DIR}/bin/polly --clear --install --config Release --toolchain osx-10-13
# Copy and install
rsync -c "_install/osx-10-13/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1DecodeCAPI.a"
rsync -c "_install/osx-10-13/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1EncodeCAPI.a"
rsync -c "_install/osx-10-13/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/osx-10-13/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/osx-10-13/lib/libMach1DecodePositionalCAPI.a"
# Upload built libs
aws s3 sync _install/osx-10-13/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/osx-10-13/lib --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE