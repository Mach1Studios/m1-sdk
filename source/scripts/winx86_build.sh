#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain vs-15-2017
rsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Release/"
rsync -c "_install/vs-15-2017/lib-shared" "../mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Release/"
rsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/windows-x86"
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017
rsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Debug/"
rsync -c "_install/vs-15-2017/lib-shared" "../mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Debug/"
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64
rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Release/"
rsync -c "_install/vs-15-2017-win64/lib-shared" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Release/"
rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/windows-x86_64"
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017-win64
rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Debug/"
rsync -c "_install/vs-15-2017-win64/lib-shared" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Debug/"
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-mt
rsync -c "_install/vs-15-2017-mt/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Release/"
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017-mt
rsync -c "_install/vs-15-2017-mt/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Debug/"
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64-mt
rsync -c "_install/vs-15-2017-win64-mt/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Release/"
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017-win64-mt
rsync -c "_install/vs-15-2017-win64-mt/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Debug/"

# Upload built libs
aws s3 sync "_install/vs-15-2017/lib/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Release --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017/lib-shared/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Release --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017/lib/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/windows-x86 --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017/lib/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Debug --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017/lib-shared/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Debug --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017-win64/lib/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Release --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017-win64/lib-shared/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Release --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017-win64/lib/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/windows-x86_64 --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017-win64/lib/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Debug --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017-win64/lib-shared/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Debug --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017-mt/lib/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Release --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017-mt/lib/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Debug --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017-win64-mt/lib/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Release --cache-control no-cache --metadata-directive REPLACE --profile mach1
aws s3 sync "_install/vs-15-2017-win64-mt/lib/" s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Debug --cache-control no-cache --metadata-directive REPLACE --profile mach1

# Update examples
# rsync -c "_install/vs-15-2017/lib-shared" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Windows/x86"
# rsync -c "_install/vs-15-2017/lib-shared" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Windows/x86"
# rsync -c "_install/vs-15-2017/lib" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Win32"
# rsync -c "_install/vs-15-2017/lib-shared" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Win32"
# rsync -c "_install/vs-15-2017/lib" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/vs/x86"
# rsync -c "_install/vs-15-2017-win64/lib-shared" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecodeTest/Assets/Mach1/Plugins/Windows/x86_64"
# rsync -c "_install/vs-15-2017-win64/lib-shared" "../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/Windows/x86_64"
# rsync -c "_install/vs-15-2017-win64/lib" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Win64"
# rsync -c "_install/vs-15-2017-win64/lib-shared" "../examples/mach1spatial-c/Unreal Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/ThirdParty/Mach1/bin/Win64"
# rsync -c "_install/vs-15-2017-win64/lib" "../examples/mach1spatial-c/openframeworks/ofxMach1/libs/libmach1/lib/vs/x64"
