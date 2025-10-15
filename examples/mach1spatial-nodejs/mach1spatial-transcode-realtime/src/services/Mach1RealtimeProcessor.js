/**
 * Mach1RealtimeProcessor Service
 * 
 * Handles real-time processing of audio through Mach1Transcode and Mach1Decode
 */

import { Mach1Transcode, Mach1Decode } from '../Mach1APIs.js';

export class Mach1RealtimeProcessor {
  constructor() {
    this.m1Transcode = null;
    this.m1Decode = null;
    this.isInitialized = false;
    this.inputFormat = '5.1_C';
    this.outputFormat = 'M1Spatial-8';
    this.decodeMode = 1;
  }

  /**
   * Initialize Mach1 APIs
   */
  async initialize() {
    try {
      // Initialize Mach1 APIs using mock implementations
      this.m1Transcode = new Mach1Transcode();
      this.m1Transcode.setPlatformType('Mach1PlatformType.Mach1PlatformDefault');
      this.m1Transcode.setInputFormat(this.inputFormat);
      this.m1Transcode.setOutputFormat(this.outputFormat);
      this.m1Transcode.setInputNumChannels(6); // 5.1 input
      this.m1Transcode.setOutputNumChannels(8); // M1Spatial-8 output
      this.m1Transcode.processConversionPath();

      this.m1Decode = new Mach1Decode();
      this.m1Decode.setPlatformType('Mach1PlatformType.Mach1PlatformDefault');
      this.m1Decode.setDecodeMode(this.decodeMode);
      this.m1Decode.setFilterSpeed(0.5);

      this.isInitialized = true;
      return true;
    } catch (error) {
      console.error('Failed to initialize Mach1 APIs:', error);
      return false;
    }
  }

  /**
   * Process audio buffer through Mach1Transcode and Mach1Decode
   */
  processAudioBuffer(inputData, outputData, bufferSize, orientation = { yaw: 0, pitch: 0, roll: 0 }) {
    if (!this.isInitialized || !this.m1Transcode || !this.m1Decode) {
      return false;
    }

    try {
      // Step 1: Transcode input to Mach1Spatial format
      const transcodedData = [];
      for (let channel = 0; channel < 8; channel++) {
        transcodedData[channel] = new Float32Array(bufferSize);
      }

      this.m1Transcode.processConversion(inputData, transcodedData, bufferSize);

      // Step 2: Update orientation
      this.m1Decode.setRotationDegrees(orientation);

      // Step 3: Decode Mach1Spatial to stereo
      const decodedData = [];
      for (let channel = 0; channel < 8; channel++) {
        decodedData[channel] = new Float32Array(bufferSize);
      }

      this.m1Decode.decodeBuffer(transcodedData, decodedData, bufferSize);

      // Step 4: Mix down to stereo output
      for (let i = 0; i < bufferSize; i++) {
        let leftMix = 0;
        let rightMix = 0;

        for (let channel = 0; channel < 8; channel++) {
          if (channel % 2 === 0) {
            leftMix += decodedData[channel][i] || 0;
          } else {
            rightMix += decodedData[channel][i] || 0;
          }
        }

        if (outputData[0]) outputData[0][i] = leftMix;
        if (outputData[1]) outputData[1][i] = rightMix;
      }

      return true;
    } catch (error) {
      console.error('Error processing audio buffer:', error);
      return false;
    }
  }

  /**
   * Get processing statistics
   */
  getStats() {
    return {
      isInitialized: this.isInitialized,
      inputFormat: this.inputFormat,
      outputFormat: this.outputFormat,
      decodeMode: this.decodeMode
    };
  }
}
