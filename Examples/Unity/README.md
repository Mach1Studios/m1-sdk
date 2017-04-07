### Mach1 Unity Examples
### 
### 
### Current Audio Formats (all algorithms contained inside M1DSPAlgorithms.cs, all functions/features inside M1Base.cs):
 - M1Spatial [Multichannel] / 8Channel [M1SpatialDecode.cs]
 - M1Horizon [Multichannel] / 4Channel [M1HorizonDecode.cs] 
 - M1StSP [Stereo Spatializer] / 2Channel [M1StSP.cs]
 - M1Horizon [Pair] / 4Pair [DirectionalSound.cs][DEPRECATED]

#### Directions:
After importing all scripts into your /Scripts/ directory, add the desired format as a component to your object.

It is recommended to increase your project's `ProjectSettings` `Max Real Voices` count to accomondate the maximum number of consecutive M1 Spatial objects in your project (by a multiple of 8 or 16). 

#### Directories Explained:
 - M1UnityDecodeExample - Uses `M1DSPAlgorithms.cs` to playback our VVBP method of audio, allowing spatial sound mixes to be correctly decoded to stereo output (headphones) with Yaw,Pitch,Roll inputs. 
 - M1 Example Player - Deprecated version of a VideoPlayer example that decodes the 4Pair playback of our VVBP deliverables. 

#### Further Use:
 - All Mach1 Spatial audio formats are contained in the `M1DSPAlgorithms.cs` as functions that can be called with custom use. Simply return those coefficients as audio source volume multipliers to use the desired format, reference the `M1SpatialDecode.cs` as an example. 
 - Non-Pair formats will usually be accompanied by an additional Stereo layer that can use traditional Unity stereo audio implementation, it just needs to be called with the same event as the spatial audio.