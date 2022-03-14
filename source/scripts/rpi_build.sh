#!/bin/bash

# RPI CROSS COMPILE BUILDS
cd ${TRAVIS_BUILD_DIR}/source
${POLLY_SOURCE_DIR}/bin/polly --clear --install --config Release --toolchain raspberrypi2-cxx11
${POLLY_SOURCE_DIR}/bin/polly --clear --install --config Release --toolchain raspberrypi3-cxx11
rsync -c "_install/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a"
rsync -c "_install/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a"
rsync -c "_install/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a"
rsync -c "_install/raspberrypi2-cxx11/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1Decode-minifiedCAPI.a"
rsync -c "_install/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a"
rsync -c "_install/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a"
rsync -c "_install/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a"
rsync -c "_install/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a"
rsync -c "_install/raspberrypi3-cxx11/lib/libMach1Decode-minifiedCAPI.a" "../mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1Decode-minifiedCAPI.a"
# Upload built libs
aws s3 sync "_install/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1DecodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1EncodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1TranscodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi2-cxx11/lib/libMach1DecodePositionalCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1DecodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1EncodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1TranscodeCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/arm-chipsets/raspberrypi3-cxx11/lib/libMach1DecodePositionalCAPI.a --cache-control no-cache --metadata-directive REPLACE --profile mach1
