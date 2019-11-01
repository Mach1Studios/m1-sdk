set ANDROID_NDK=D:\SDKs\android-ndk-r16b
set ANDROID_NDK_r16b=D:\SDKs\android-ndk-r16b
set ANDROID_HOME=D:\SDKs\android
set CMAKE_HOME=%ANDROID_HOME%/cmake/3.6.4111459/bin
set PATH=%PATH%;%CMAKE_HOME%;%ANDROID_HOME%;%ANDROID_HOME%/platform-tools;%ANDROID_HOME%/tools;%ANDROID_NDK%;%ANDROID_NDK%/prebuilt/windows-x86_64/bin;

python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r16b-api-21-armeabi-v7a-clang-libcxx

del /f /s /q _android 1>nul 
rmdir /s /q _android

mkdir _android
cd _android

mkdir armeabi-v7a

cd %~dp0

xcopy _install\android-ndk-r16b-api-21-armeabi-v7a-clang-libcxx\lib _android\armeabi-v7a
pause
