#!/bin/bash

# 
# Mach1
# Download audio files for web examples
# 

if [[ $PWD/ = */mach1spatial-web/* ]]
then 
	echo $PWD
else
	echo "You are in the wrong directory!"
	echo $PWD
	exit
fi

mkdir -p audio/mono
mkdir -p audio/stereo
mkdir -p audio/quad
mkdir -p audio/m1spatial
mkdir -p video

cd audio/mono
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/mono/1.mp3

cd ../stereo
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Orchestral_Stereo_L.ogg
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Orchestral_Stereo_R.ogg
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Orchestral_Stereo.ogg

cd ../quad
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/quad/guitar-m1horizon.ogg

cd ../m1spatial
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/T1.ogg
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/T2.ogg
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/T3.ogg
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/T4.ogg
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/B5.ogg
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/B6.ogg
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/B7.ogg
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/m1spatial/B8.ogg

cd ../../video
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/video/video.webm