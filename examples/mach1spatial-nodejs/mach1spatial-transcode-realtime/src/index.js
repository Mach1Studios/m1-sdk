/**
 * Mach1 Real-time Transcode & Decode Example
 * 
 * This example demonstrates real-time processing of audio through:
 * 1. Mach1Transcode: Converting input audio to Mach1Spatial format
 * 2. Mach1Decode: Decoding Mach1Spatial audio with orientation control
 * 
 * The processing happens on a per-buffer basis in real-time using Web Audio API
 */

// Import Mach1 APIs - using implementations for demonstration
import { Mach1Transcode, Mach1Decode } from './Mach1APIs.js';
import { Mach1RealtimeProcessor } from './services/Mach1RealtimeProcessor.js';

// Global state
let audioContext = null;
let sourceNode = null;
let processorNode = null;
let gainNode = null;
let isPlaying = false;
let isLoaded = false;
let currentAudioBuffer = null;

// Mach1 API instances
let m1Transcode = null;
let m1Decode = null;
let realtimeProcessor = null;

// Processing state
let inputFormat = '5.1_C';
let outputFormat = 'M1Spatial-8';
let decodeMode = 1; // M1Spatial-8 decode mode

// Orientation state
let yaw = 0.0;
let pitch = 0.0;
let roll = 0.0;

// Debug state
let showDebug = false;
let debugInfo = [];

// Global controls for DatGUI
window.controls = {
  yawMultiplier: 1.0,
  pitchMultiplier: 1.0,
  rollMultiplier: 1.0,
  filterSpeed: 0.5,
  showDebug: false
};

/**
 * Initialize the Mach1 APIs
 */
async function initializeMach1() {
  try {
    // Initialize real-time processor
    realtimeProcessor = new Mach1RealtimeProcessor();
    const initialized = await realtimeProcessor.initialize();
    
    if (!initialized) {
      throw new Error('Failed to initialize real-time processor');
    }

    // Initialize Mach1Transcode
    m1Transcode = new Mach1Transcode();
    m1Transcode.setPlatformType('Mach1PlatformType.Mach1PlatformDefault');
    m1Transcode.setInputFormat(inputFormat);
    m1Transcode.setOutputFormat(outputFormat);
    m1Transcode.setInputNumChannels(6); // 5.1 input
    m1Transcode.setOutputNumChannels(8); // M1Spatial-8 output
    m1Transcode.processConversionPath();

    // Initialize Mach1Decode
    m1Decode = new Mach1Decode();
    m1Decode.setPlatformType('Mach1PlatformType.Mach1PlatformDefault');
    m1Decode.setDecodeMode(decodeMode);
    m1Decode.setFilterSpeed(0.5);

    addDebugInfo('Mach1 APIs initialized successfully');
    addDebugInfo(`Input format: ${inputFormat} (6 channels)`);
    addDebugInfo(`Output format: ${outputFormat} (8 channels)`);
    addDebugInfo(`Decode mode: ${decodeMode}`);

    return true;
  } catch (error) {
    addDebugInfo(`Error initializing Mach1 APIs: ${error.message}`, 'error');
    return false;
  }
}

/**
 * Load and decode audio file
 */
async function loadAudioFile() {
  const fileInput = document.getElementById('audioFile');
  const file = fileInput.files[0];
  
  if (!file) {
    updateStatus('Please select an audio file', 'error');
    return;
  }

  try {
    updateStatus('Loading audio file...', 'warning');
    
    // Create audio context if not exists
    if (!audioContext) {
      audioContext = new (window.AudioContext || window.webkitAudioContext)();
    }

    // Decode audio file
    const arrayBuffer = await file.arrayBuffer();
    currentAudioBuffer = await audioContext.decodeAudioData(arrayBuffer);
    
    addDebugInfo(`Audio loaded: ${file.name}`);
    addDebugInfo(`Sample rate: ${currentAudioBuffer.sampleRate} Hz`);
    addDebugInfo(`Duration: ${currentAudioBuffer.duration.toFixed(2)}s`);
    addDebugInfo(`Channels: ${currentAudioBuffer.numberOfChannels}`);

    // Initialize Mach1 APIs
    const mach1Initialized = await initializeMach1();
    if (!mach1Initialized) {
      updateStatus('Failed to initialize Mach1 APIs', 'error');
      return;
    }

    isLoaded = true;
    updateStatus('Audio loaded successfully', 'success');
    document.getElementById('playButton').disabled = false;
    
  } catch (error) {
    addDebugInfo(`Error loading audio: ${error.message}`, 'error');
    updateStatus('Failed to load audio file', 'error');
  }
}

/**
 * Toggle playback
 */
async function togglePlayback() {
  if (!isLoaded || !audioContext) return;

  if (isPlaying) {
    stopPlayback();
  } else {
    startPlayback();
  }
}

/**
 * Start audio playback with real-time processing
 */
async function startPlayback() {
  if (!currentAudioBuffer || !m1Transcode || !m1Decode) return;

  try {
    // Resume audio context if suspended
    if (audioContext.state === 'suspended') {
      await audioContext.resume();
    }

    // Create source node
    sourceNode = audioContext.createBufferSource();
    sourceNode.buffer = currentAudioBuffer;
    sourceNode.loop = true;

    // Create gain node for output
    gainNode = audioContext.createGain();
    gainNode.gain.value = 0.5; // Reduce volume to prevent clipping

    // Create processor node for real-time processing
    processorNode = audioContext.createScriptProcessor(4096, 6, 2); // 6 input channels, 2 output channels

    // Set up the processing function
    processorNode.onaudioprocess = (event) => {
      processAudioBuffer(event);
    };

    // Connect the audio graph
    sourceNode.connect(processorNode);
    processorNode.connect(gainNode);
    gainNode.connect(audioContext.destination);

    // Start playback
    sourceNode.start(0);
    isPlaying = true;

    updateStatus('Playing with real-time processing', 'success');
    document.getElementById('playButton').textContent = 'Pause';
    document.getElementById('stopButton').disabled = false;

    addDebugInfo('Playback started with real-time processing');

  } catch (error) {
    addDebugInfo(`Error starting playback: ${error.message}`, 'error');
    updateStatus('Failed to start playback', 'error');
  }
}

/**
 * Stop audio playback
 */
function stopPlayback() {
  if (sourceNode) {
    sourceNode.stop();
    sourceNode.disconnect();
    sourceNode = null;
  }

  if (processorNode) {
    processorNode.disconnect();
    processorNode = null;
  }

  isPlaying = false;
  updateStatus('Stopped', 'warning');
  document.getElementById('playButton').textContent = 'Play';
  document.getElementById('stopButton').disabled = true;

  addDebugInfo('Playback stopped');
}

/**
 * Real-time audio processing function
 * This is where the magic happens - processing each buffer through Mach1Transcode and Mach1Decode
 */
function processAudioBuffer(event) {
  if (!realtimeProcessor || !m1Transcode || !m1Decode) return;

  try {
    const inputBuffer = event.inputBuffer;
    const outputBuffer = event.outputBuffer;
    const bufferLength = inputBuffer.length;
    const inputChannels = inputBuffer.numberOfChannels;
    const outputChannels = outputBuffer.numberOfChannels;

    // Prepare input data for processing
    const inputData = [];
    for (let channel = 0; channel < inputChannels; channel++) {
      inputData[channel] = inputBuffer.getChannelData(channel);
    }

    // Prepare output data
    const outputData = [];
    for (let channel = 0; channel < outputChannels; channel++) {
      outputData[channel] = outputBuffer.getChannelData(channel);
    }

    // Use real-time processor for the complete pipeline
    const success = realtimeProcessor.processAudioBuffer(
      inputData,
      outputData,
      bufferLength,
      { yaw, pitch, roll }
    );

    if (!success) {
      // Fallback to direct processing if real-time processor fails
      processAudioBufferDirect(inputData, outputData, bufferLength);
    }

    // Update debug info periodically
    if (Math.random() < 0.01) { // 1% chance to update debug info
      addDebugInfo(`Processing buffer: ${bufferLength} samples`);
      addDebugInfo(`Input channels: ${inputChannels}, Output channels: ${outputChannels}`);
      addDebugInfo(`Orientation: Yaw=${yaw.toFixed(1)}°, Pitch=${pitch.toFixed(1)}°, Roll=${roll.toFixed(1)}°`);
    }

  } catch (error) {
    addDebugInfo(`Error in audio processing: ${error.message}`, 'error');
  }
}

/**
 * Direct audio processing fallback
 */
function processAudioBufferDirect(inputData, outputData, bufferLength) {
  try {
    // Process through Mach1Transcode
    const transcodedData = [];
    for (let channel = 0; channel < 8; channel++) { // M1Spatial-8 has 8 channels
      transcodedData[channel] = new Float32Array(bufferLength);
    }

    // Call Mach1Transcode processConversion for each buffer
    m1Transcode.processConversion(inputData, transcodedData, bufferLength);

    // Update orientation
    m1Decode.setRotationDegrees({ x: yaw, y: pitch, z: roll });

    // Process through Mach1Decode
    const decodedData = [];
    for (let channel = 0; channel < 8; channel++) {
      decodedData[channel] = new Float32Array(bufferLength);
    }

    // Call Mach1Decode decodeBuffer for each buffer
    m1Decode.decodeBuffer(transcodedData, decodedData, bufferLength);

    // Mix down to stereo output
    for (let i = 0; i < bufferLength; i++) {
      let leftMix = 0.0;
      let rightMix = 0.0;

      // Mix all decoded channels to stereo
      for (let channel = 0; channel < 8; channel++) {
        if (channel % 2 === 0) {
          leftMix += decodedData[channel][i];
        } else {
          rightMix += decodedData[channel][i];
        }
      }

      if (outputData[0]) outputData[0][i] = leftMix;
      if (outputData[1]) outputData[1][i] = rightMix;
    }

  } catch (error) {
    addDebugInfo(`Error in direct audio processing: ${error.message}`, 'error');
  }
}

/**
 * Update orientation values
 */
function updateOrientation(newYaw, newPitch, newRoll) {
  yaw = newYaw * window.controls.yawMultiplier;
  pitch = newPitch * window.controls.pitchMultiplier;
  roll = newRoll * window.controls.rollMultiplier;

  // Update display
  document.getElementById('yawValue').textContent = yaw.toFixed(1);
  document.getElementById('pitchValue').textContent = pitch.toFixed(1);
  document.getElementById('rollValue').textContent = roll.toFixed(1);
}

/**
 * Reset orientation to center
 */
function resetOrientation() {
  updateOrientation(0, 0, 0);
  document.getElementById('yawSlider').value = 0;
  document.getElementById('pitchSlider').value = 0;
  document.getElementById('rollSlider').value = 0;
  updateSliderDisplays();
}

/**
 * Update slider displays
 */
function updateSliderDisplays() {
  document.getElementById('yawControl').textContent = document.getElementById('yawSlider').value;
  document.getElementById('pitchControl').textContent = document.getElementById('pitchSlider').value;
  document.getElementById('rollControl').textContent = document.getElementById('rollSlider').value;
}

/**
 * Update status display
 */
function updateStatus(message, type = '') {
  const statusElement = document.getElementById('status');
  statusElement.textContent = message;
  statusElement.className = `status ${type}`;
}

/**
 * Add debug information
 */
function addDebugInfo(message, type = '') {
  const timestamp = new Date().toLocaleTimeString();
  const debugItem = document.createElement('div');
  debugItem.className = `debug-item ${type}`;
  debugItem.textContent = `[${timestamp}] ${message}`;
  
  const debugContent = document.getElementById('debugContent');
  debugContent.appendChild(debugItem);
  
  // Keep only last 50 debug items
  while (debugContent.children.length > 50) {
    debugContent.removeChild(debugContent.firstChild);
  }
  
  // Auto-scroll to bottom
  debugContent.scrollTop = debugContent.scrollHeight;
}

/**
 * Toggle debug panel visibility
 */
function toggleDebug() {
  const debugPanel = document.getElementById('debugPanel');
  debugPanel.classList.toggle('hidden');
  window.controls.showDebug = !debugPanel.classList.contains('hidden');
}

// Event listeners
document.addEventListener('DOMContentLoaded', () => {
  // File input change
  document.getElementById('audioFile').addEventListener('change', loadAudioFile);

  // Orientation sliders
  document.getElementById('yawSlider').addEventListener('input', (e) => {
    updateOrientation(parseFloat(e.target.value), pitch, roll);
    updateSliderDisplays();
  });

  document.getElementById('pitchSlider').addEventListener('input', (e) => {
    updateOrientation(yaw, parseFloat(e.target.value), roll);
    updateSliderDisplays();
  });

  document.getElementById('rollSlider').addEventListener('input', (e) => {
    updateOrientation(yaw, pitch, parseFloat(e.target.value));
    updateSliderDisplays();
  });

  // Initialize slider displays
  updateSliderDisplays();

  // Initialize orientation
  updateOrientation(0, 0, 0);

  addDebugInfo('Real-time Transcode & Decode example loaded');
  addDebugInfo('Load an audio file to begin processing');
});

// Global functions for HTML buttons
window.loadAudioFile = loadAudioFile;
window.togglePlayback = togglePlayback;
window.stopPlayback = stopPlayback;
window.resetOrientation = resetOrientation;
window.toggleDebug = toggleDebug;
