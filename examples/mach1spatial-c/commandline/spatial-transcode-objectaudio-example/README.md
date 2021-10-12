### Mach1Transcode 

### Introduction
This is a stripped down transcoding commmandline example, please reference or link/use the full Mach1 Transcode commandline tool located at [m1-sdk/binaries/executables/](https://github.com/Mach1Studios/m1-sdk/tree/master/binaries/executables). This example allows any format input with the default example of a Mach1 Spatial input and transcodes the audio data to the ADM BWF configuration specifically for the end-target of Dolby Atmos. The resulting BWF PCM file contains expected ADM related metadata written within the audio data for ingestion to Dolby Atmos Conversion Tool or other 3rd party ADM parsing workflows.

#### Setup
 - install homebrew: https://brew.sh/
 - `brew install libsndfile` _(project is currently linked to 1.0.31)_
 - Open each xcode project

#### Example Arguement
`-in-fmt M1Spatial -in-file "m1-spatial-8ch.wav" -out-fmt DolbyAtmosSevenOneTwo -out-file "output-DolbyAtmosConfigured-ADM-BWF.wav" -out-file-chans 0`

