REM
REM Mach1
REM Download audio files for web examples
REM

cd %~dp0
mkdir audio\mono
mkdir audio\stereo
mkdir audio\quad
mkdir audio\m1spatial
mkdir video

cd audio\mono
powershell -Command "Invoke-WebRequest -OutFile 1.mp3 http://dev.mach1.xyz/mach1-sdk-sample-audio/web/mono/1.mp3"

cd ..\stereo
powershell -Command "Invoke-WebRequest -OutFile M1_SDKDemo_Orchestral_Stereo_L.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Orchestral_Stereo_L.ogg"
powershell -Command "Invoke-WebRequest -OutFile M1_SDKDemo_Orchestral_Stereo_R.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Orchestral_Stereo_R.ogg"
powershell -Command "Invoke-WebRequest -OutFile M1_SDKDemo_Orchestral_Stereo.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Orchestral_Stereo.ogg"

cd ..\quad
powershell -Command "Invoke-WebRequest -OutFile guitar-m1horizon.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/quad/guitar-m1horizon.ogg"

cd ..\m1spatial
powershell -Command "Invoke-WebRequest -OutFile T1.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/T1.ogg"
powershell -Command "Invoke-WebRequest -OutFile T2.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/T2.ogg"
powershell -Command "Invoke-WebRequest -OutFile T3.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/T3.ogg"
powershell -Command "Invoke-WebRequest -OutFile T4.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/T4.ogg"
powershell -Command "Invoke-WebRequest -OutFile B1.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/B5.ogg"
powershell -Command "Invoke-WebRequest -OutFile B2.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/B6.ogg"
powershell -Command "Invoke-WebRequest -OutFile B3.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/B7.ogg"
powershell -Command "Invoke-WebRequest -OutFile B4.ogg http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/B8.ogg"

cd ..\..\video
powershell -Command "Invoke-WebRequest -OutFile video.webm http://dev.mach1.xyz/mach1-sdk-sample-audio/web/video/video.webm"