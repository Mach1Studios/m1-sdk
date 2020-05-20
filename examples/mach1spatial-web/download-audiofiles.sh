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

cd audio/mono
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/mono/1.mp3

cd ../stereo
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Orchestral_Stereo_L.ogg
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Orchestral_Stereo_R.ogg
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/stereo/M1_SDKDemo_Orchestral_Stereo.ogg

cd ../quad
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/web/quad/guitar-m1horizon.ogg

