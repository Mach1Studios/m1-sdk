# Mach1 Real-time Transcode & Decode Example

This Node.js web example demonstrates real-time processing of audio through Mach1Transcode and Mach1Decode APIs in a web browser environment.

## Features

- **Real-time Processing**: Processes audio on a per-buffer basis using Web Audio API
- **Mach1Transcode**: Converts input audio (5.1 surround) to Mach1Spatial format (M1Spatial-8)
- **Mach1Decode**: Decodes Mach1Spatial audio with real-time orientation control
- **Interactive Controls**: Real-time orientation adjustment with sliders
- **Debug Panel**: Comprehensive debugging information and processing statistics
- **File Support**: Load any audio file supported by the browser
- **Mach1 API Implementation**: Includes Mach1 APIs for demonstration and testing

## Installation and First Run

All shell commands must be run only in the project directory

### Option 1: Using Python HTTP Server (Recommended)
Navigate to the examples directory and start a simple HTTP server:

```sh
cd examples
python3 -m http.server 3000
```

The example will be available at:
- `http://localhost:3000/index.html` (main demo with ScriptProcessorNode)

### Option 2: Using Node.js Development Server
Install package dependencies:

```sh
npm install
```

Run the example:

```sh
npm run example
```

The example will be available at `http://localhost:3000`

## Using Real Mach1 APIs

To use the actual Mach1 APIs instead of the demonstration implementations:

1. **Replace API Imports**: In `src/index.js`, change:
   ```javascript
   import { Mach1Transcode, Mach1Decode } from './Mach1APIs.js';
   ```
   to:
   ```javascript
   const Mach1Transcode = require('../lib/Mach1Transcode.js');
   const Mach1Decode = require('../lib/Mach1Decode.js');
   ```

2. **Update Webpack Config**: Ensure the webpack configuration includes proper fallbacks for Node.js modules.

3. **Handle WASM Loading**: The real Mach1 APIs require proper WASM file loading and initialization.

## API Reference

This example demonstrates the following Mach1 API methods:

### Mach1Transcode:
- `setInputFormat()` / `setOutputFormat()`
- `processConversionPath()`
- `processConversion()`

### Mach1Decode:
- `setDecodeMode()`
- `setRotationDegrees()`
- `decodeBuffer()`
