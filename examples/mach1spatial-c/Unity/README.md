### Mach1 Unity Examples
### 
### 
### Current Audio Formats (all algorithms contained inside `libMach1Decode`, all functions/features inside `M1Base`):
 - M1Spatial [Multichannel] / 8Channel [M1SpatialDecode.cs] {Yaw,Pitch,Roll + Positional}
 - M1Horizon [Multichannel] / 4Channel [M1HorizonDecode.cs] {Yaw + Positional}
 - M1StSP [Stereo Spatializer] / 2Channel [M1StSP.cs] {Yaw, Pitch, Roll + Positional}
 - M1Horizon [Pair] / 4Pair [DirectionalSound.cs][DEPRECATED]

#### Directions:
After importing all scripts into the desired project's directory, add the desired format as a component to your object.

It is recommended to increase your project's `ProjectSettings` `Max Real Voices` count to accomondate the maximum number of consecutive M1 Spatial objects in your project (by a multiple of 8 or 16). 

#### Directories Explained:
 - M1UnityDecodeTest - Uses `libMach1Decode` to playback our VVBP spatial audio formats, allowing spatial sound mixes to be correctly decoded to stereo output (headphones) with Yaw,Pitch,Roll + Positional data inputs. 
 - M1 Video Player - An example 360 VideoPlayer that uses `libMach1Decode` to playback our VVBP spatial audio formats. 

#### Further Use:
 - All Mach1 Spatial audio formats are contained in the `libMach1Decode` as functions that can be called with custom use. Simply return those coefficients as audio source volume multipliers to use the desired format, reference the `M1SpatialDecode.cs` as an example. 
 - Non-Pair formats will usually be accompanied by an additional Stereo layer that can use traditional Unity stereo audio implementation, it just needs to be called with the same event as the spatial audio.
 - `BlendMode` function is an experimental feature that allows pre-rendered room modeling by using the player's positional data along with orientation data to correctly mix between a pre-rendered center soundscape and a pre-rendered bound/wall soundscape. Please contact ```dylan@mach1studios.com``` for references on best mixing practices and use.