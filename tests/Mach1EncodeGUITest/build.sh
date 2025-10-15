#!/bin/bash

# Mach1Encode GUI Test Tool Build Script
# This script helps build the GUI test tool with proper dependencies

set -e  # Exit on any error

echo "Building Mach1Encode GUI Test Tool..."

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: CMakeLists.txt not found. Please run this script from the Mach1EncodeGUITest directory."
    exit 1
fi

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
echo "Building..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "Build complete!"
echo "Run the tool with: ./Mach1EncodeGUITest"
