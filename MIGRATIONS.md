# Migrations
Currently the Mach1 Spatial SDK has gone through some revisions to improve on its usability based on usage and feedback. This document is to help describe any breaking changes to help anyone migrating from an older version of the repo.

#### Stable legacy versions:
[Version 2](https://github.com/Mach1Studios/m1-sdk)
[Version 1](https://github.com/Mach1Studios/m1-sdk/tree/1.2)

## Migration to 4.0+
Version 4 represents a **major architectural shift** introducing a **source-code** for the APIs for the first time. Previously, the SDK was distributed only as pre-compiled binary libraries. V4 now provides full source code access, comprehensive build systems, and development infrastructure in an effort to help this project shift towards a community maintainable model.

### **SDK Distribution Model Change**
- **Source Code Added**: Complete `libmach1spatial/` source directory with all API implementations
- **Build System**: Full CMake and Makefile build system for cross-platform compilation
- **Development Tools**: CI/CD, testing frameworks, documentation generation, and development scripts
- **Open Development**: Access to modify, build, and contribute to the SDK source code

### **Library Integration Changes**
#### **CMakeLists.txt Integration Options**:
```cmake
# NEW (v4): Option 1 - Build from source (recommended)
add_subdirectory(libmach1spatial)
target_link_libraries(${PROJECT_NAME} PRIVATE M1Decode M1Transcode)

# LEGACY (v3): Still supported - Link pre-built libraries
find_library(MACH1DECODE_LIBRARY
             NAMES Mach1DecodeCAPI libMach1DecodeCAPI
             HINTS ${MACH1SPATIAL_LIBS_PATH}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE ${MACH1DECODE_LIBRARY})
```

#### **Header Includes**:
```cpp
# NEW (v4): Include from source (recommended)
#include <Mach1Decode.h>  // From libmach1spatial/api_decode/include/

# LEGACY (v3): Still available - Include from binary distribution if you compile libs and includes
#include "Mach1Decode.h"  // From mach1spatial-libs/include/
```

### **API Enhancements**

#### **Mach1Decode API**:
```cpp
# NEW (v4): Enhanced template-based instantiation
Mach1Decode<float> m1Decode;
m1Decode.setDecodeMode(M1DecodeSpatial_14);

# LEGACY (v3): Direct instantiation (still supported)
Mach1Decode m1Decode;
m1Decode.setDecodeAlgoType(Mach1DecodeAlgoSpatial);
m1Decode.beginBuffer();
m1Decode.setRotationDegrees(orientation);
coeffs = m1Decode.decodeCoeffs();
m1Decode.endBuffer();
```

#### **Mach1Encode API**:
```cpp
# NEW (v4): Simplified enum values
if(inputMode == Stereo) inputMode = LCR;
if(outputMode == M1Spatial_8) outputMode = M1Spatial_4;

# LEGACY (v3): Fully qualified enum names (still supported)
if(inputMode == Mach1EncodeInputModeStereo) inputMode = Mach1EncodeInputModeLCR;
if(outputMode == Mach1EncodeOutputModeM1Spatial_8) outputMode = Mach1EncodeOutputModeM1Horizon_4;
```

#### **Mach1Transcode API**:
```cpp
# NEW (v4): Enhanced template-based
Mach1Transcode<float> m1transcode;
outFmt = m1transcode.getFormatFromString("M1Spatial-14");

# LEGACY (v3): Direct instantiation (still supported)
Mach1Transcode m1transcode;
outFmt = m1transcode.getFormatFromString("M1Spatial");
```

### **Build System Enhancements**

#### **New Build Options**:
```makefile
# NEW (v4): Source-based cmake build (recommended)
cmake . -Bbuild
cmake --build build

# LEGACY (v3): Platform-specific compilation (still available via manual linking)
# macOS:
g++ -o app src/main.cpp \
    ../../../../mach1spatial-libs/xcode/include/Mach1Decode.cpp \
    -I../../../../mach1spatial-libs/xcode/include \
    -L../../../../mach1spatial-libs/xcode/lib \
    -lMach1DecodeCAPI
```

### **Development Features Added**
Version 4 introduces comprehensive development infrastructure:
- **Complete Testing Suite**: Unit tests for all APIs with automated CI/CD
- **Documentation System**: Doxygen-based documentation generation
- **Cross-Platform Building**: Unified CMake system for all platforms
- **Development Scripts**: Formatting, testing, and build automation
- **Git Submodules**: Proper dependency management
- **Examples Enhancement**: Source-based examples with better platform support

### **New Capabilities in v4**
1. **Source Code Access**: Full ability to modify and customize the SDK
2. **Template Support**: Enhanced C++ template-based APIs
3. **Build Flexibility**: Choice between source builds or binary linking
4. **Development Tools**: Complete CI/CD and testing infrastructure
5. **Documentation**: Auto-generated API documentation
6. **Cross-Platform**: Unified build system across all platforms
7. **Community Contributions**: Open development model with proper tooling

### **Migration Benefits**
- [ ] **Full Source Access**: Modify and customize SDK behavior as needed
- [ ] **Better Integration**: CMake-based builds integrate better with existing projects
- [ ] **Enhanced APIs**: Template support and simplified enum usage
- [ ] **Development Tools**: Testing, documentation, and CI/CD infrastructure
- [ ] **Backward Compatibility**: Existing v3 binary integration still works
- [ ] **Better Examples**: More comprehensive examples with source builds
- [ ] **Community**: Ability to contribute back to the SDK development

### **Migration Path**
v4 maintains **backward compatibility** with v3, so migration is optional but recommended:

1. **Immediate**: Continue using existing v3 binary integration (no changes required)
2. **Gradual**: Start using source builds for new features while keeping existing code
3. **Full Migration**: Transition to template-based APIs and source-based builds

## Migration to 3.0+
Version 3 removes some old deprecated variables and focuses on clearer and more concise function/variable terminology; and introduces CI/CD to make it easier in the future for community contributions to this project allowing the codebase to self test and distribute with the goal of becoming easier to maintain over time. Major changes for each API as follows: 
- All APIs: Use specific vector based `Mach1Spatial-n` formats where `n` is the number of channels in the configuration of that format, typically derived or inspired by from platonic solid shapes.
- Mach1Encode API: Azimuth, Elevation and Diverge are the only terms used for setting a source sound's direction and signal distribution. 
- Mach1Decode API: Verbose description of orientation data is required within function name.
- Mach1Transcode API: Swapped inputs/outputs of matrix columns/rows, inputs are now rows to allow inline description of an input channel's "location" via Mach1Encode API style Azimuth/Elevation/Diverge as well as the coefficients directly to the output channels. 