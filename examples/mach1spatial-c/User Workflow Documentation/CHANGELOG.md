# Changelog
All notable changes to the Mach1 Spatial System will be documented in this file. [Unreleased] section documents upcoming features to be expected for the workflow. 

## [Unreleased]
Monitor: Mach1 Horizon Mode: disables pitch & roll on M1-Monitor to safely check Mach1 Horizon Mixes in sessions
Monitor: arrow keys set Yaw value to direction
PreviewPlayer: FuMa support
PreviewPlayer: Youtube Binaural Filter audition
Panner: Dynamic Mach1 Encoding Normalization applied to M1-Panners

## [1.3.2] - 2017-05-07
Panner: 1.1.1.0
Monitor: 1.0.5.4
Transcoder: 1.1.0.1
VideoPlayer: 0.9.5
 
### Added
- Panner: Improved encoding math to Mach1Spatial by introducing more isotropic encoding behavior to the cuboid shape
- Panner: Diverge effects Z/Pitch as a multiplier

## [1.3.1] - 2017-04-23
Panner: 1.1.0.1
Monitor: 1.0.5.4
Transcoder: 1.1.0.1
VideoPlayer: 0.9.5
 
### Added
- Transcoder: M1SDK audio output
- Transcoder: added Mach1 spatial output support for sideloading into SamsungVR
- VideoPlayer: added overlay image when in flattened view
- Transcoder: added Mach1Spatial -> TBE wav conversion
- Transcoder: added Mach1Spatial + Stereo -> Surround formats wav conversions
- Panner: extended gain knob range
- Transcoder: if encoding/transcoding to youtube, static stereo mix will be downmixed to mono and added to W channel of ambisonic
 
### BugFix
- Transcoder: fixed more of the output option types
- Panner: fixed encoding issues on QUAD inputs and outputs
- VideoPlayer: fixed issues with bluetooth/serial parsing for hardware headtracking support
- Panner: fixed quad>quad UI issues

## [1.3.0] - 2017-04-02
Panner: 1.1.0.0
Monitor: 1.0.5.2
Transcoder: 1.1.0.0
VideoPlayer: 0.9.3

### Added
- Panner: QUAD input added
- Panner: AFORMAT input added
- Panner: BFORMAT input added (FOA: ambiX)
- Transcoder: Support for Optional Static Stereo / Headlocked Stereo inputs
- PreviewPlayer: Some formats now audition Static Stereo / Headlocked Stereo encodings from Transcoder
- Panner: improved encoding for all input types
- Transcoder: M1SDK audio output

### BugFix
- Panner: fix QUAD output swap channel issues
- Panner: fixed GUI issues with clamping the Panner UI Dots
- Panner: optimized processBlock
- Transcoder: fixed more of the output option types

## [1.2.0] - 2017-03-01
Panner: 1.0.7.3
Monitor: 1.0.5.2
Transcoder: 1.0.6.2
VideoPlayer: 0.9.3

### Added
- Panner/Monitor: Shift+Mouse for fine-tune value changing on knobs/sliders
- Monitor: Option+MouseClick to reset Yaw/Pitch/Roll sliders to default
- PreviewPlayer: Added video/audio scrub bar

### Changed
- Panner: ReticleGrid displays front/back UI with 0/180 label
- Panner/Monitor: fixed bug where spawning too many Panner instances could break StereoSafe mode
- Monitor/VideoPlayer: fixed OSC between both clients

## [1.1.0] - 2017-02-14
Panner: 1.0.7.2
Monitor: 1.0.5.1
Transcoder: 1.0.6.0

### Added
- Panner/Monitor: Stereo Safe Mode: When activated from Monitor, all M1-Panner instances in DAW session with encode to mono and preserve gain ratios based on automation and pan settings. 
(for exporting audio that will be safe for 360 video players that do not support spatial audio yet)
- Panner: Overlay Guidelines: While overlay reticle is moving displays guidelines vertically and horizontally.
- Transcoder: safely convert 8channel uncompressed audio to 8channel AAC/M4A using Mach1 Spatial Output Type, typical AAC encoders will re-order channels and apply filters.
- Monitor: Front/Back Foldback Mode: When activated; front channels and rear channels are downmixed to stereo L & R for checking front to back gain ratios.

### Changed
Panner: Slight UI changes
Monitor: Slight UI changes
VideoPlayer: renamed application
VideoPlayer: fixed codesign issue
VideoPlayer: fixed misuse of UDP


## [1.0.0] - 2018-02-05
Panner: 1.0.7.0
Monitor: 1.0.4.9
Transcoder: 1.0.6.0

### Added
Monitor: monitor mode UI
Transcoder: bin cleanup
Transcoder: documentation
Panner: option + click to reset values to default
Transcoder: parse out dangerous characters during transcoding
Panner: maximum bounds of Diverge preserved during automation
Panner: documentation
Monitor: documentation
Transcoder: better output type handling based on user inputs
Transcoder: Pro Tools HD channel input detection to preserve channel order correctly

### Changed
PreviewPlayer: updated to latest libMach1Decode
Transcoder: fixed issue with some outputs breaking transcoding
Transcoder: updated to latest m1-fmtconv
VideoPlayer: updated to latest libMach1Decode
Panner: fixed overlay snap when using with Pro Tools HD Native Video window
PreviewPlayer: fixed issue with crashing on audio decoding

### Removed
Panner: stepped rotation automation to host DAW for pretection from DAWs that cannot support endless rotary UI
