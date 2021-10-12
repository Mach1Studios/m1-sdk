## Mach1 Unreal Engine Integration Examples

### INSTALLATION
#### Installing Mach1 Decode Plugin (Mach1 Spatial VVBP Decoding)
 - It is recommended to install the source code plugin as supplied. The binary staged plugins are difficult to install to the engine from version to version so the source code is supplied for use with the UAT.
 - Installation README supplied with plugin directory.  

### Unreal Engine 4 Setup Notes
- Set Project setting to play sound with volume = 0 (Virtualize Silent Sounds)

### Unreal Engine 5 Setup Notes
- Apply the following settings to all audio files used in the M1SpatialActor object
- Change the `Virtualization Mode` to `Play when Silent`
- Check the `Priority` `Bypass Volume Scale for Priority` to true

![UE5-VoiceSettings](/.README/UE5-VoiceSettings.png)

### CONTACT:
Mach1:  
whatsup@mach1.tech
