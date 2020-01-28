REM
REM Mach1
REM Download audio files for web examples
REM

cd %~dp0
mkdir audio\mono
mkdir audio\stereo
mkdir audio\quad

cd audio\mono
powershell -Command "Invoke-WebRequest -OutFile 1.mp3 http://dev.mach1.xyz/mach1-sdk-sample-audio/web/mono/1.mp3"

cd ..\stereo
powershell -Command "Invoke-WebRequest -OutFile M1_SDKDemo_Electronic_Stereo_L.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Electronic_Stereo_L.ogg"
powershell -Command "Invoke-WebRequest -OutFile M1_SDKDemo_Electronic_Stereo_R.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Electronic_Stereo_R.ogg"
powershell -Command "Invoke-WebRequest -OutFile M1_SDKDemo_Electronic_Stereo.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Electronic_Stereo.ogg"

cd ..\quad
powershell -Command "Invoke-WebRequest -OutFile guitar-m1horizon.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/quad/guitar-m1horizon.ogg"
