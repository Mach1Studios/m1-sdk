python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r11c-api-21-armeabi-v7a

del /f /s /q _android 1>nul
rmdir /s /q _android

mkdir _android
cd _android

mkdir armeabi-v7a

cd %~dp0

xcopy _install\android-ndk-r11c-api-21-armeabi-v7a\lib _android\armeabi-v7a
pause
