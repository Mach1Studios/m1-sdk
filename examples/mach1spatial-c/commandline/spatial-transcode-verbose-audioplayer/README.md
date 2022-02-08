# Mach1Transcode & Mach1Decode Verbose Audio Player

## Introduction
This is another transcoding commmandline example that passes resulting buffers to a Mach1Decode enabled audio mixer via RtAudio, this is written more verbosely to make it easier to see a full design.
`Input Buffer` -> `Transcode the buffer` -> `Decode the buffer`

_Please reference or link/use the full feature Mach1 Transcode commandline tool located at [m1-sdk/executables/](https://github.com/Mach1Studios/m1-sdk/tree/master/executables)_

## Setup
 - install homebrew: https://brew.sh/
 - `brew install libsndfile` _(project is currently linked to 1.0.31)_
 - `brew install rt-audio` _(project is currently linked to 5.0.1_1)_
 - Open each xcode project

## Usage
Example command:
`./spatial-transcode-verbose-audioplayer -in-file "/path/to/input-fiveone.wav" -in-fmt FiveOneFilm_Cinema -out-file "/path/to/output-m1spatial.wav" -out-fmt M1Spatial -out-file-chans 0`
