# m1-transcode executable binaries

## Usecases

* Commandline execution for unix/linux/windows environments
* Use to convert to/from any Mach1 Spatial format
* Use to convert to/from any multichannel surround format (by using Mach1 Spatial as the hidden layer)
* Use to convert to/from any spatial/ambisonic format (by using Mach1 Spatial as the hidden layer)
* Input custom multichannel configurations of any size to build custom conversion and pipelines
* Analyze for top/bottom soundfield signal differences for use cases that need to safely downmix height
* Analyze ADM metadata within audio files
* Faster than real time for execution on server side

## Usage

##### Example in command line for converting Mach1Spatial mix to First Order ambisonics: ACNSN3D
`./m1-transcode fmtconv -in-file /path/to/file.wav -in-fmt M1Spatial -out-fmt ACNSN3D -out-file /path/to/output.wav -out-file-chans 0`

##### Example in command line for converting 7.1 film mix to Mach1Spatial

`./m1-transcode fmtconv -in-file /path/to/file.wav -in-fmt SevenOnePT_Cinema -out-fmt Mach1Spatial -out-file /path/to/output.wav`

### Custom Format/Configuration

`./m1-transcode fmtconv -in-file /path/to/16channel.wav -in-fmt TTPoints -in-json /path/to/16ChannelDescription.json -out-file /path/to/output-m1spatial.wav -out-fmt M1Spatial -out-file-chans 0`

Input JSON description of the surround/spatial soundfield setup per your design and input it with the -in-json arguement for any custom input or output transcoding.

The `-in-fmt` or `-out-fmt` as `TTPoints` can be used to set a custom json format.

You can also find an example custom `.json` file here: [reference/Custom16Channel.json](reference/Custom16Channel.json)

#### ADM Extractor
`./m1-transcode fmtconv -in-file /path/to/input-ADM.wav -in-fmt SevenOneFour -out-file /path/to/output.wav -extract-metadata -out-fmt M1Spatial -out-file-chans 0`

`-extract-metadata` will dump any found XML ADM metadata in the audio binary as a textfile with the same output name and path.

#### LFE/SUB Channel Filter

##### Example of low pass filtering every channel but the Front-Right of the Mach1 Spatial mix and outputting it to stereo.
`./m1-transcode fmtconv -in-file /path/to/input-m1spatial.wav -in-fmt M1Spatial -out-file /path/to/output-stereo.wav -lfe-sub 0,2,3,4,6,7 -out-fmt Stereo -out-file-chans 0`

Use -lfe-sub arguement to indicate which input channels you want to apply a Low Pass Filter to, the arguement exapects a list of ints with commas to separate them.

#### Mach1 Spatial Downmixer
`./m1-transcode fmtconv -in-file /path/to/input-fiveOne.wav -in-fmt FiveOneFilm_Cinema -spatial-downmixer 0.9 -out-file /path/to/output-m1spatial.wav -out-fmt M1Spatial -out-file-chans 0`

`-spatial-downmixer` arguement can be used to set this, the float after this arguement will be used as the threshold. If used this will effectively add an additional transoding after anything outputting to Mach1 Spatial to then transcode from Mach1 Spatial to Mach1 Horizon while respecting the content of the soundfield. 

## Requests

### Format Requests
Please raise an issue or email [whatsup@mach1.tech](mailto:whatsup@mach1.tech) to submit a description of a multichannel format you wish to be included. Please include: 

* Name of Format
* Number of channels
* Order of channels
* Description of channels (using our conventions below)

### Platform Requests
Please raise an issue or email [whatsup@mach1.tech](mailto:whatsup@mach1.tech)

## Mach1 Internal Angle Standard

### Coordinate / Angle / Rotation Description Expectations:

* Rotations can be individually explained per axis with signed rotations
* Rotations are explained from a center perspective point of view (FPV - First Person View)

### Mach1 YPR Polar Expectation of Describing Orientation:

#### Common use: Mach1Decode API, Mach1DecodePositional API
* Yaw   (left -> right | where rotating left is negative)
* Pitch (down -> up | where rotating down is negative)
* Roll  (top-pointing-left -> top-pointing-right | where rotating top of object left is negative)

### Mach1 AED Expectation of Describing Polar Points:

#### Common use: Mach1Encode API
* Azimuth   (left -> right | where rotating left is negative)
* Elevation (down -> up | where rotating down is negative)
* Diverge   (backward -> forward | where behind origin of Azimuth/Elevation is negative)

### Mach1 XYZ Coordinate Expectation of Vector Points:
* X (left -> right | where -X is left)
* Y (front -> back | where -Y is back)
* Z (top -> bottom | where -Z is bottom)

# License #

By downloading and/or using the Mach1 Spatial SDK, including use of any of the contents within the [mach1spatial-libs/](mach1spatial-libs), you agree to and acknowledge the terms of use set forth by the [Mach1 Spatial SDK License](https://www.mach1.tech/license). If you do not agree to the terms set forth by the [Mach1 Spatial SDK License](https://www.mach1.tech/license) you are not permitted to use, link, compile and/or distribute any of the contents of this repository.
*These terms include keeping the LICENSE.txt file in either the parent directory or working directory of the Mach1 library binary files of your project.*

Please view the [license/](license) directory for more information and proper attribution resources.