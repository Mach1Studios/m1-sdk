### Mach1 Unity Examples
### 
### 
### Current Audio Formats (all formats contained inside M1DSPAlgorithms.h):
 - M1 Square [Pair] / 4Pair [Used by DirectionalSound.cs]
 - M1 Square [Multichannel] / 4Channel [un-implemented] 
 - M1 Cube [Multichannel] / 8Channel [Used by CubeSound.cs]

#### Directories Explained:
 - M1 Example Player Cube - Uses `M1DSPAlgorithms.h` to playback our VVBP method of audio, allowing spatial sound mixes to be correctly decoded to stereo output (headphones) with Yaw,Pitch,Roll inputs. 
 - M1 Example Player - Deprecated version of a VideoPlayer example that decodes the 4Pair playback of our VVBP deliverables. 

#### Further Use:
 - All Mach1 Spatial audio formats are contained in the `M1DSPAlgorithms.h` as functions that can be called with custom use. Simply return those coefficients as audio source volume multipliers to use the desired format, reference the `CubeSound.cs` as an example. 
 - Non-Pair formats will usually be accompanied by an additional Stereo layer that can use traditional Unity stereo audio implementation, it just needs to be called with the same event as the spatial audio.