#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain vs-15-2017
rsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Release/"
aws s3 sync _install/vs-15-2017/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
rsync -c "_install/vs-15-2017/lib-shared" "../mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Release/"
aws s3 sync _install/vs-15-2017/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
rsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/windows-x86"
aws s3 sync _install/vs-15-2017/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/windows-x86 --cache-control no-cache --exclude "*-minifiedCAPI*" --metadata-directive REPLACE
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017
rsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Debug/"
aws s3 sync _install/vs-15-2017/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
rsync -c "_install/vs-15-2017/lib-shared" "../mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Debug/"
aws s3 sync _install/vs-15-2017/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64
rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Release/"
aws s3 sync _install/vs-15-2017-win64/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
rsync -c "_install/vs-15-2017-win64/lib-shared" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Release/"
aws s3 sync _install/vs-15-2017-win64/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/windows-x86_64"
aws s3 sync _install/vs-15-2017-win64/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/windows-x86_64 --cache-control no-cache --exclude "*-minifiedCAPI*" --metadata-directive REPLACE
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017-win64
rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Debug/"
aws s3 sync _install/vs-15-2017-win64/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
rsync -c "_install/vs-15-2017-win64/lib-shared" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Debug/"
aws s3 sync _install/vs-15-2017-win64/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-mt
rsync -c "_install/vs-15-2017-mt/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Release/"
aws s3 sync _install/vs-15-2017-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017-mt
rsync -c "_install/vs-15-2017-mt/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Debug/"
aws s3 sync _install/vs-15-2017-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64-mt
rsync -c "_install/vs-15-2017-win64-mt/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Release/"
aws s3 sync _install/vs-15-2017-win64-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017-win64-mt
rsync -c "_install/vs-15-2017-win64-mt/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Debug/"
aws s3 sync _install/vs-15-2017-win64-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE