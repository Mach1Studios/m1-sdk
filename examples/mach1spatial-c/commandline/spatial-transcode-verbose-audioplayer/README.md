# Mach1Transcode & Mach1Decode Verbose Audio Player

## Introduction
This is another transcoding commmandline example that passes resulting buffers to a Mach1Decode enabled audio mixer via RtAudio, this is written more verbosely to make it easier to see a full design.
`Input Buffer` -> `Transcode the buffer` -> `Decode the buffer`

_Please reference or link/use the full feature Mach1 Transcode commandline tool located at [m1-sdk/executables/](https://github.com/Mach1Studios/m1-sdk/tree/master/executables)_

## Setup
 - Run the `make setup` command from the root dir of this repo

## CMake Setup
 - `cmake . -Bbuild`
 - `cmake --build build`

## Usage
Example command:
`./spatial-transcode-verbose-audioplayer -in-file "/path/to/input-fiveone.wav" -in-fmt 5.1_C -out-file "/path/to/output-m1spatial.wav" -out-fmt M1Spatial -out-file-chans 0`
