#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain gcc
rsync -c "_install/gcc/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1DecodeCAPI.a"
rsync -c "_install/gcc/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1EncodeCAPI.a"
rsync -c "_install/gcc/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/linux/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/gcc/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/linux/lib/libMach1DecodePositionalCAPI.a"
# Upload built libs
aws s3 sync _install/gcc/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/linux/lib --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE