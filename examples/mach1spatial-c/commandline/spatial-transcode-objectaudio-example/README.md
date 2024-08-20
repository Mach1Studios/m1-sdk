### Mach1Transcode

### Introduction
This is a stripped down transcoding commmandline example, please reference or link/use the full Mach1 Transcode commandline tool located at [m1-sdk/executables/](https://github.com/Mach1Studios/m1-sdk/tree/master/executables). This example allows any format input with the default example of a Mach1 Spatial input and transcodes the audio data to the ADM BWF configuration specifically for the end-target of Dolby Atmos. The resulting BWF PCM file contains expected ADM related metadata written within the audio data for ingestion to Dolby Atmos Conversion Tool or other 3rd party ADM parsing workflows.

#### Setup

##### CMake Setup
 - `mkdir build`
 - `cd build`
 - `cmake ../`
 - `cmake --build .`

##### macOS Setup
 - install homebrew: https://brew.sh/
 - `brew install libsndfile` _(project is currently linked to 1.0.31)_
 - Open each xcode project

##### Linux Setup
- Change directory to `src/`
- Install libsndfile: http://www.mega-nerd.com/libsndfile/#Download

##### Windows Setup
- Use Powershell to run the following script from this dir: `.\setup.sh`
- Use VCPKG to install `sndfile` or manually install to the `libs` dir
- `mkdir build`
- `cd build`
- `cmake ..`
- `cmake --build .`

#### Example Arguement
`-in-fmt M1Spatial -in-file "m1-spatial-8ch.wav" -out-fmt 7.1.2_C -out-file "output-Atmos7_1_2-ADM-BWF.wav" -out-file-chans 0`

#### TODO
 - Bitdepth and samplerate matches input audiofile _(currently hardset to 48k 24bit)_
