# Mach1Transcode & Mach1Decode Audio Player

## Introduction
This is a stripped down transcoding commmandline example that passes resulting streams to a Mach1Decode enabled audio mixer via RtAudio,

_Please reference or link/use the full feature Mach1 Transcode commandline tool located at [m1-sdk/executables/](https://github.com/Mach1Studios/m1-sdk/tree/master/executables)_

## Setup
 - Run the `make setup` command from the root dir of this repo

## CMake Setup
 - `cmake . -Bbuild`
 - `cmake --build build`

## Usage
Example command:
`./spatial-transcode-audioplayer -in-file "/path/to/input-fiveone.wav" -in-fmt 5.1_C -out-file "/path/to/output-m1spatial.wav" -out-fmt M1Spatial -out-file-chans 0`
