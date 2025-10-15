# Mach1Transcode & Mach1Decode Real-Time Audio Player

## Introduction
This is a **REAL-TIME** transcoding commandline example that properly tests Mach1Transcode and Mach1Decode performance in real-time audio processing.

**Key Differences from Other Examples:**
- **Actually calls `Mach1Transcode.processConversion()` in real-time** (unlike other examples that pre-calculate coefficients)
- **Handles multiple buffer conversions** during the transcode step
- **Tests real-time performance** of both Mach1Transcode and Mach1Decode
- **Monitors CPU usage and buffer performance** in real-time
- **Processes each audio buffer individually** through the complete pipeline

## What This Example Tests

This example addresses the reported issue that "Mach1Transcode does not work in real time" by:

1. **Real-time Buffer Processing**: Each audio buffer is processed through `Mach1Transcode.processConversion()` in the audio callback
2. **Multiple Conversion Handling**: Detects and handles cases where multiple buffer conversions occur during transcode
3. **Performance Monitoring**: Tracks processing time, CPU usage, and buffer underruns/overruns
4. **Complete Pipeline**: Input → Mach1Transcode → Mach1Decode → Stereo Output

## Setup
- Run the `make setup` command from the root dir of this repo

## CMake Setup
- `cmake . -Bbuild`
- `cmake --build build`

## Usage
Example command:
```bash
./spatial-transcode-realtime-example -in-file "/path/to/input-fiveone.wav" -in-fmt 5.1_C -yaw 90.0 -pitch 15.0 -roll 0.0
```

## Real-time Controls
- `w/s` - pitch up/down
- `a/d` - yaw left/right  
- `z/x` - roll left/right
- `q` - quit

## Performance Monitoring
The example provides real-time performance statistics:
- Samples processed
- Average processing time per buffer
- CPU usage percentage
- Buffer underruns/overruns

## Technical Details

### Real-time Processing Flow
1. **Audio Input**: Read audio buffer from file
2. **Mach1Transcode**: Call `processConversion()` on each buffer
3. **Multiple Conversions**: Handle intermediate buffer conversions if needed
4. **Mach1Decode**: Apply decode to transcoded buffer
5. **Stereo Output**: Output final stereo mix

### Buffer Conversion Handling
The example detects when multiple conversion steps are needed:
```cpp
std::vector<int> conversionPath = m1transcode.getFormatConversionPath();
if (conversionPath.size() > 2) {
    // Handle multiple conversion steps
    // Copy transcoded output to intermediate buffers
}
```

### Performance Monitoring
```cpp
auto startTime = std::chrono::high_resolution_clock::now();
m1transcode.processConversion(inputBuffers, transcodedBuffers, nBufferFrames);
auto endTime = std::chrono::high_resolution_clock::now();
// Calculate and track processing time
```

## Comparison with Other Examples

| Example | Real-time Mach1Transcode | Performance Monitoring | Multiple Conversions |
|---------|---------------------------|------------------------|---------------------|
| spatial-transcode-verbose-audioplayer | ❌ Pre-calculated | ❌ | ❌ |
| spatial-transcode-audioplayer | ❌ Pre-calculated | ❌ | ❌ |
| **spatial-transcode-realtime-example** | ✅ **Real-time** | ✅ **Yes** | ✅ **Yes** |

## Troubleshooting

If you experience real-time issues:
1. Check the performance statistics output
2. Monitor CPU usage - should be < 50% for stable real-time
3. Watch for buffer underruns/overruns
4. Try different buffer sizes if needed
5. Ensure your system can handle the processing load

This example will help identify if Mach1Transcode performance issues are due to:
- CPU limitations
- Buffer size problems  
- Multiple conversion overhead
- System audio driver issues
