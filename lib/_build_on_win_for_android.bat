set ANDROID_NDK=D:\SDKs\android-ndk-r16b
set ANDROID_NDK_r16b=D:\SDKs\android-ndk-r16b
set ANDROID_HOME=D:\SDKs\android
set CMAKE_HOME=%ANDROID_HOME%/cmake/3.6.4111459/bin
set PATH=%PATH%;%CMAKE_HOME%;%ANDROID_HOME%;%ANDROID_HOME%/platform-tools;%ANDROID_HOME%/tools;%ANDROID_NDK%;%ANDROID_NDK%/prebuilt/windows-x86_64/bin;

python ../../polly/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx
python ../../polly/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
python ../../polly/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-clang-libcxx
python ../../polly/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-64-clang-libcxx

del /f /s /q _android 1>nul
rmdir /s /q _android

mkdir ..\binaries\android-arm64-v8a
mkdir ..\binaries\android-armeabi-v7a
mkdir ..\binaries\android-x86
mkdir ..\binaries\android-x86_64

rem TODO: make single parent android dir and reference that via the following:
rem mkdir arm64-v8a
rem mkdir armeabi
rem mkdir armeabi-v7a
rem mkdir mips
rem mkdir mips64
rem mkdir x86
rem mkdir x86_64

cd %~dp0

xcopy _install\android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx\lib ..\binaries\android-arm64-v8a\lib /Y
xcopy _install\android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx\lib ..\binaries\android-armeabi-v7a\lib /Y
xcopy _install\android-ndk-r16b-api-21-x86-clang-libcxx\lib ..\binaries\android-x86\lib /Y
xcopy _install\android-ndk-r16b-api-21-x86-64-clang-libcxx\lib ..\binaries\android-x86_64\lib /Y
pause
