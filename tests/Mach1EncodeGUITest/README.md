# Mach1Encode GUI Test Tool

A comprehensive graphical testing and visualization tool for the Mach1Encode API. This tool provides an interactive interface for testing all Mach1Encode parameters and visualizing the resulting gain coefficients and spatial configurations.

## Features


### Visualization
- **Gain Matrix**: Tabular view of all gain coefficients with color coding
- **3D Spatial Points**: 2D projection of 3D spatial point positions
- **Gain Bars**: Visual representation of gain values per channel
- **Statistics**: Comprehensive analysis of gain distribution
- **Channel Analysis**: Detailed breakdown of each output channel

### Real-time Updates
- Auto-update mode for real-time parameter changes
- Manual update button for precise control
- Performance timing display
### Dependencies
The tool automatically downloads ImGui using CMake's FetchContent. Other dependencies should be installed via your system's package manager:

**macOS (using Homebrew):**
```bash
brew install glfw glew
```

**Ubuntu/Debian:**
```bash
sudo apt-get install libglfw3-dev libglew-dev
```

**Windows (using vcpkg):**
```bash
vcpkg install glfw3 glew
```

