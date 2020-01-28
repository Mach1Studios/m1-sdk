REM
REM Mach1
REM Download audio files for web examples
REM

cd %~dp0
mkdir app\src\main\assets

cd app\src\main\assets
powershell -Command "Invoke-WebRequest -OutFile Counting-8ch.aac http://dev.mach1.xyz/mach1-sdk-sample-audio/android/mach1-decode-example/assets/Counting-8ch.aac"
powershell -Command "Invoke-WebRequest -OutFile Counting-8ch.m4a http://dev.mach1.xyz/mach1-sdk-sample-audio/android/mach1-decode-example/assets/Counting-8ch.m4a"
powershell -Command "Invoke-WebRequest -OutFile Guitar-8ch.aac http://dev.mach1.xyz/mach1-sdk-sample-audio/android/mach1-decode-example/assets/Guitar-8ch.aac"
powershell -Command "Invoke-WebRequest -OutFile Guitar-8ch.m4a http://dev.mach1.xyz/mach1-sdk-sample-audio/android/mach1-decode-example/assets/Guitar-8ch.m4a"