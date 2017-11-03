python ../../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r11c-api-21-arm64-v8a
python ../../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r11c-api-21-armeabi
python ../../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r11c-api-21-armeabi-v7a
python ../../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r11c-api-21-mips
python ../../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r11c-api-21-mips64
python ../../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r11c-api-21-x86
python ../../../polly-master/bin/polly.py --clear --install --config Release --toolchain android-ndk-r11c-api-21-x86-64

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

xcopy _install\android-ndk-r11c-api-21-arm64-v8a\lib _android\arm64-v8a
xcopy _install\android-ndk-r11c-api-21-armeabi\lib _android\armeabi
xcopy _install\android-ndk-r11c-api-21-armeabi-v7a\lib _android\armeabi-v7a
xcopy _install\android-ndk-r11c-api-21-mips\lib _android\mips
xcopy _install\android-ndk-r11c-api-21-mips64\lib _android\mips64
xcopy _install\android-ndk-r11c-api-21-x86\lib _android\x86
xcopy _install\android-ndk-r11c-api-21-x86-64\lib _android\x86_64
pause
