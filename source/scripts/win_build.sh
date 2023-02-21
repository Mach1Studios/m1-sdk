#!/bin/bash

while getopts 'uh:' OPTION; do
	case "$OPTION" in
		u)
			upload_artifacts=ON
			echo "Uploading binaries to aws"
			;;
		h)
			echo "script usage: $(basename \$0) [-u] [-h]" >&2
			exit 1
			;;
		?)
			echo "script usage: $(basename \$0) [-u] [-h]" >&2
			exit 1
			;;
	esac
done
shift "$(($OPTION -1))"

if [[ "$PWD" == *source ]]
then
	echo "Script called from correct path: $PWD"
	# dynamic win32
	cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -DBUILD_SHARED_LIBS=1 -A Win32 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017/Release
	cmake --build _builds/vs-15-2017 --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017/Release/lib-shared/* "../mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Release/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
	cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -DBUILD_SHARED_LIBS=1 -A Win32 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017/Debug
	cmake --build _builds/vs-15-2017 --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017/Debug/lib-shared/* "../mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Debug/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Dynamic/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
	# dynamic x64
	cmake . -B_builds/vs-15-2017-win64 -G "Visual Studio 15 2017" -DBUILD_SHARED_LIBS=1 -A x64 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64/Release
	cmake --build _builds/vs-15-2017-win64 --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017-win64/Release/lib-shared/* "../mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Release/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017-win64/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
	cmake . -B_builds/vs-15-2017-win64 -G "Visual Studio 15 2017" -DBUILD_SHARED_LIBS=1 -A x64 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64/Debug
	cmake --build _builds/vs-15-2017-win64 --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017-win64/Debug/lib-shared/* "../mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Debug/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017-win64/lib-shared/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Dynamic/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
	# MD win32
	cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -A Win32 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017/Release
	cmake --build _builds/vs-15-2017 --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017/Release/lib/* "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Release/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
	rsync -c --mkpath _install/vs-15-2017/lib/* "../mach1spatial-libs/windows-x86"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/windows-x86 --cache-control no-cache --exclude "*-minifiedCAPI*" --metadata-directive REPLACE
	fi
	cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -A Win32 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017/Debug
	cmake --build _builds/vs-15-2017 --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017/Debug/lib/* "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Debug/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MD/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
	# MD x64
	cmake . -B_builds/vs-15-2017-win64 -G "Visual Studio 15 2017" -A x64 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64/Release
	cmake --build _builds/vs-15-2017-win64 --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017-win64/Release/lib/* "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Release/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017-win64/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
	rsync -c --mkpath _install/vs-15-2017-win64/lib/* "../mach1spatial-libs/windows-x86_64"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017-win64/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/windows-x86_64 --cache-control no-cache --exclude "*-minifiedCAPI*" --metadata-directive REPLACE
	fi
	cmake . -B_builds/vs-15-2017-win64 -G "Visual Studio 15 2017" -A x64 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64/Debug
	cmake --build _builds/vs-15-2017-win64 --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017-win64/Debug/lib/* "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Debug/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017-win64/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MD/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
	# MT win32
	cmake . -B_builds/vs-15-2017-mt -G "Visual Studio 15 2017" -A Win32 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-mt/Release
	cmake --build _builds/vs-15-2017-mt --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017-mt/Release/lib/* "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Release/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
	cmake . -B_builds/vs-15-2017-mt -G "Visual Studio 15 2017" -A Win32 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-mt/Debug
	cmake --build _builds/vs-15-2017-mt --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017-mt/Debug/lib/* "../mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Debug/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86/lib/Static/MT/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
	# MT x64
	cmake . -B_builds/vs-15-2017-win64-mt -G "Visual Studio 15 2017" -A x64 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64-mt/Release
	cmake --build _builds/vs-15-2017-win64-mt --config Release --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017-win64-mt/Release/lib/* "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Release/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017-win64-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Release --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
	# TODO: figure out why this is not compiling d.lib style
	cmake . -B_builds/vs-15-2017-win64-mt -G "Visual Studio 15 2017" -A x64 -DCOMPILE_MT=1 -DCMAKE_INSTALL_PREFIX=_install/vs-15-2017-win64-mt/Debug
	cmake --build _builds/vs-15-2017-win64-mt --config Debug --target install -- //nologo //verbosity:quiet //clp:ErrorsOnly
	rsync -c --mkpath _install/vs-15-2017-win64-mt/Debug/lib/* "../mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Debug/"
	if [[ $upload_artifacts == "ON" ]]
	then
		aws s3 sync _install/vs-15-2017-win64-mt/lib/ s3://${AWS_DEPLOY_BUCKET}/mach1spatial-libs/vs-15-2017-x86_64/lib/Static/MT/Debug --exclude "*-minifiedCAPI*" --cache-control no-cache --metadata-directive REPLACE
	fi
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi