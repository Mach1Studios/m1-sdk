# ofxAudioDecoder is an [openFrameworks](http://openframeworks.cc/) wrapper for [libaudiodecoder](https://github.com/asantoni/libaudiodecoder/)

On OSX it can load .mp3, .m4a, .wav, and some other formats. Instead of being used for playback, it's meant for analysis. Because libaudiodecoder also supports Windows, with the addition of the Windows-specific sources and the creation of a Windows-specific project file ofxAudioDecoder should also run on windows.

## Example

The example project loads a file `demo.mp3` from the `data/` folder and draws the left and right channels into an `ofMesh`. You may drag and drop a file onto the OF app to load it and render the waveform.