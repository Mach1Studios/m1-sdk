#!/bin/bash


if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	# dynamic win32
	cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -DBUILD_SHARED_LIBS=1 -A Win32 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017
	cmake --build _builds/vs-15-2017 --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017/lib-shared" "../mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Release/"
	aws s3 sync _install/vs-15-2017/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -DBUILD_SHARED_LIBS=1 -A Win32 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017
	cmake --build _builds/vs-15-2017 --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017/lib-shared" "../mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Debug/"
	aws s3 sync _install/vs-15-2017/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	# dynamic x64
	cmake . -B_builds/vs-15-2017-win64 -G "Visual Studio 15 2017" -DBUILD_SHARED_LIBS=1 -A x64 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64
	cmake --build _builds/vs-15-2017-win64 --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017-win64/lib-shared" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Release/"
	aws s3 sync _install/vs-15-2017-win64/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	cmake . -B_builds/vs-15-2017-win64 -G "Visual Studio 15 2017" -DBUILD_SHARED_LIBS=1 -A x64 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64
	cmake --build _builds/vs-15-2017-win64 --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017-win64/lib-shared" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Debug/"
	aws s3 sync _install/vs-15-2017-win64/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	# MD win32
	cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -A Win32 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017
	cmake --build _builds/vs-15-2017 --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Release/"
	aws s3 sync _install/vs-15-2017/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	rsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/windows-x86"
	aws s3 sync _install/vs-15-2017/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/windows-x86 --cache-control no-cache --exclude "*-minifiedCAPI*" --metadata-directive REPLACE
	cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -A Win32 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017
	cmake --build _builds/vs-15-2017 --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Debug/"
	aws s3 sync _install/vs-15-2017/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	# MD x64
	cmake . -B_builds/vs-15-2017-win64 -G "Visual Studio 15 2017" -A x64 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64
	cmake --build _builds/vs-15-2017-win64 --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Release/"
	aws s3 sync _install/vs-15-2017-win64/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/windows-x86_64"
	aws s3 sync _install/vs-15-2017-win64/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/windows-x86_64 --cache-control no-cache --exclude "*-minifiedCAPI*" --metadata-directive REPLACE
	cmake . -B_builds/vs-15-2017-win64 -G "Visual Studio 15 2017" -A x64 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64
	cmake --build _builds/vs-15-2017-win64 --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017-win64/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Debug/"
	aws s3 sync _install/vs-15-2017-win64/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	# MT win32
	cmake . -B_builds/vs-15-2017-mt -G "Visual Studio 15 2017" -A Win32 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-mt
	cmake --build _builds/vs-15-2017-mt --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017-mt/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Release/"
	aws s3 sync _install/vs-15-2017-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	cmake . -B_builds/vs-15-2017-mt -G "Visual Studio 15 2017" -A Win32 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-mt
	cmake --build _builds/vs-15-2017-mt --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017-mt/lib" "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Debug/"
	aws s3 sync _install/vs-15-2017-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	# MT x64
	cmake . -B_builds/vs-15-2017-win64-mt -G "Visual Studio 15 2017" -A x64 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64-mt
	cmake --build _builds/vs-15-2017-win64-mt --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017-win64-mt/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Release/"
	aws s3 sync _install/vs-15-2017-win64-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	cmake . -B_builds/vs-15-2017-win64-mt -G "Visual Studio 15 2017" -A x64 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64-mt
	cmake --build _builds/vs-15-2017-win64-mt --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c "_install/vs-15-2017-win64-mt/lib" "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Debug/"
	aws s3 sync _install/vs-15-2017-win64-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi