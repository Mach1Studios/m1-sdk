#!/bin/bash

# 
# Mach1
# Download audio files for Android examples
# 

if [[ $PWD/ = */mach1-decode-example/* ]]
then 
	echo $PWD
else
	echo "You are in the wrong directory!"
	echo $PWD
	exit
fi

mkdir -p app/src/main/assets

cd app/src/main/assets
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/android/mach1-decode-example/assets/Counting-8ch.aac
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/android/mach1-decode-example/assets/Counting-8ch.m4a
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/android/mach1-decode-example/assets/Guitar-8ch.aac
wget http://dev.mach1.xyz/mach1-sdk-sample-audio/android/mach1-decode-example/assets/Guitar-8ch.m4a