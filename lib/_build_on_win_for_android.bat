set ANDROID_NDK=D:\SDKs\android-ndk-r16b
set ANDROID_NDK_r16b=D:\SDKs\android-ndk-r16b
set ANDROID_HOME=D:\SDKs\android
set CMAKE_HOME=%ANDROID_HOME%/cmake/3.6.4111459/bin
set PATH=%PATH%;%CMAKE_HOME%;%ANDROID_HOME%;%ANDROID_HOME%/platform-tools;%ANDROID_HOME%/tools;%ANDROID_NDK%;%ANDROID_NDK%/prebuilt/windows-x86_64/bin;

python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx
python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx
python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-clang-libcxx
python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-x86-64-clang-libcxx

del /f /s /q _android 1>nul
rmdir /s /q _android

mkdir _android
cd _android

mkdir arm64-v8a
mkdir armeabi
mkdir armeabi-v7a
mkdir mips
mkdir mips64
mkdir x86
mkdir x86_64

cd %~dp0

xcopy _install\android-ndk-r16b-api-21-arm64-v8a-neon-clang-libcxx\lib _android\arm64-v8a
xcopy _install\android-ndk-r16b-api-21-armeabi-v7a-neon-clang-libcxx\lib _android\armeabi-v7a
xcopy _install\android-ndk-r16b-api-21-x86-clang-libcxx\lib _android\x86
xcopy _install\android-ndk-r16b-api-21-x86-64-clang-libcxx\lib _android\x86_64
pause
