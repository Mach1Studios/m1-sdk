# Mach1 Spatial Web Examples

## Audio Directory

1. use `./download-audiofiles.sh` (or `./download-audiofiles.bat` if on WIN) on this directory to download some prepared debug audio/video.

2a. Place M1Encode audio file(s) in this [audio] directory per input channel type

Example path:
`audio/mono/1.ogg`

2b. Place M1Decode audio files in correct output type directory

Example paths:
`audio/m1spatial/...ogg`

#### Mach1DecodeAlgoSpatial: 
```
T1.ogg
T2.ogg
T3.ogg
T4.ogg
B5.ogg
B6.ogg
B7.ogg
B8.ogg
```

3. Either serve the entire directory (or files needed) or run `http-server` to locally serve the examples