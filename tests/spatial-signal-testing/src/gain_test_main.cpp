#include "RtAudioChain.h"
#include "SineWaveLink.h"
#include "PeakTrackerLink.h"
#include "GainLink.h"
#include "EncodeLink.h"
#include "DecodeLink.h"

#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <cmath>
#include <iomanip> // Required for std::fixed and std::setprecision

float calculateRMS(const float* samples, int numSamples) {
    float sumSquares = 0.0f;
    for (int i = 0; i < numSamples; i++) {
        sumSquares += samples[i] * samples[i];
    }
    return sqrt(sumSquares / numSamples);
}

float amplitudeToDecibels(float amplitude) {
    if (amplitude <= 0.0f) return -100.0f;
    return 20.0f * log10(amplitude);
}

int main(int argc, char *argv[]) {
    std::cout << "\n=== Mach1 Encode->Decode Gain Test ===" << std::endl;
    std::cout << "Testing: azimuth=0°, diverge=100%, 8ch Mach1Spatial, decode yaw=0°" << std::endl;
    std::cout << "Expected: Near unity gain when azimuth == yaw" << std::endl;

    Mach1::RtAudioChain chain{};
    Mach1::PeakTrackerLink peak_tracker_link[3];
    Mach1::SineWaveLink sine_wave_link{};
    Mach1::EncodeLink encode_link{};
    Mach1::DecodeLink decode_link{};
    Mach1::GainLink gain_link{};
    
    // Setup signal chain: sine -> peak[0] -> encode -> peak[1] -> decode -> peak[2]
    chain.AddLink(&sine_wave_link);
    chain.AddLink(&peak_tracker_link[0]); // Input measurement
    chain.AddLink(&encode_link);
    chain.AddLink(&peak_tracker_link[1]); // Encoded signal measurement
    chain.AddLink(&decode_link);
    chain.AddLink(&peak_tracker_link[2]); // Output measurement
    chain.AddLink(&gain_link); // Output gain control

    // Test configuration
    const int M1_FORMAT_CHANNEL_COUNT = 8;
    const float azimuth = 0.0f;           // Azimuth = 0°
    const float elevation = 0.0f;         // Elevation = 0°
    const float diverge = 1.0f;           // 100% diverge
    const float decode_yaw = 0.0f;        // Yaw = 0° (should match azimuth)
    const float decode_pitch = 0.0f;      // Pitch = 0°
    const float decode_roll = 0.0f;       // Roll = 0°
    const float test_frequency = 1000.0f; // 1kHz test tone
    const float input_gain_db = -12.0f;   // Safe test level
    const float test_duration = 3.0f;     // 3 second test

    std::cout << "\nConfiguration:" << std::endl;
    std::cout << "  Azimuth: " << azimuth << "°" << std::endl;
    std::cout << "  Elevation: " << elevation << "°" << std::endl;
    std::cout << "  Diverge: " << (diverge * 100.0f) << "%" << std::endl;
    std::cout << "  Decode Yaw: " << decode_yaw << "°" << std::endl;
    std::cout << "  Decode Pitch: " << decode_pitch << "°" << std::endl;
    std::cout << "  Decode Roll: " << decode_roll << "°" << std::endl;
    std::cout << "  Spatial Format: " << M1_FORMAT_CHANNEL_COUNT << " channel" << std::endl;
    std::cout << "  Test Frequency: " << test_frequency << " Hz" << std::endl;
    std::cout << "  Input Gain: " << input_gain_db << " dB" << std::endl;
    std::cout << "  Test Duration: " << test_duration << "s" << std::endl;

    // Configure audio chain
    chain.SetInputChannelCount(1);
    chain.SetOutputChannelCount(2);
    chain.SetAudioBufferInputChannelCount(M1_FORMAT_CHANNEL_COUNT);
    chain.SetAudioBufferOutputChannelCount(M1_FORMAT_CHANNEL_COUNT);
    chain.SetSampleRate(48000);
    chain.SetBufferSize(512);

    // Configure peak trackers
    peak_tracker_link[0].SetName("Input");
    peak_tracker_link[0].SetOutputChannelCount(1);
    peak_tracker_link[1].SetName("Encoded");
    peak_tracker_link[1].SetOutputChannelCount(M1_FORMAT_CHANNEL_COUNT);
    peak_tracker_link[2].SetName("Decoded");
    peak_tracker_link[2].SetOutputChannelCount(2);

    // Configure sine wave generator
    sine_wave_link.SetFrequency(test_frequency);
    sine_wave_link.SetGainDecibels(input_gain_db);
    sine_wave_link.SetChannelMask(~0); // Fill all channels

    // Configure encode
    encode_link.SetInputMode(Mach1EncodeInputMode::Mono);
    encode_link.SetOutputMode(M1Spatial_8);
    encode_link.SetAzimuthDegrees(azimuth);
    encode_link.SetElevationDegrees(elevation);
    encode_link.SetDiverge(diverge);
    encode_link.SetIsotropicMode(false);
    encode_link.SetEqualPowerMode(true);
    encode_link.SetOrbitRotation(0.0f);
    encode_link.SetStereoSpread(0.5f);
    encode_link.SetAutoOrbit(true);
    encode_link.GeneratePointResults();

    // Configure decode
    decode_link.SetDecodeMode(M1DecodeSpatial_8);
    decode_link.SetRotationDegrees({decode_yaw, decode_pitch, decode_roll});
    decode_link.SetPlatformType(Mach1PlatformDefault);
    decode_link.SetFilterSpeed(0.95f);
    decode_link.SetPointCount(encode_link.GetPointsCount());

    // Configure output gain (mute for automated test)
    gain_link.SetGain(0.0f);

    std::cout << "\nStarting test..." << std::endl;

    // Start the audio chain
    try {
        chain.Start();

        // Let it run for the specified duration
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(test_duration * 1000)));

        std::cout << "\nStopping audio chain..." << std::endl;
        
        // Stop the audio chain
        chain.Stop();
        
        std::cout << "Audio chain stopped successfully." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception during audio processing: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\nTest completed. Analyzing results..." << std::endl;

    // Get measurements from peak trackers
    float input_peak = peak_tracker_link[0].GetMaxPeak(0);
    float output_left_peak = peak_tracker_link[2].GetMaxPeak(0);
    float output_right_peak = peak_tracker_link[2].GetMaxPeak(1);
    
    // Calculate average output for stereo
    float output_peak_avg = (output_left_peak + output_right_peak) / 2.0f;

    // Convert to dB
    float input_db = amplitudeToDecibels(input_peak);
    float output_left_db = amplitudeToDecibels(output_left_peak);
    float output_right_db = amplitudeToDecibels(output_right_peak);
    float output_avg_db = amplitudeToDecibels(output_peak_avg);

    // Calculate gain difference
    float gain_diff_db = output_avg_db - input_db;

    std::cout << "\n=== Test Results ===" << std::endl;
    std::cout << "Input Peak: " << std::fixed << std::setprecision(3) << input_peak 
              << " (" << input_db << " dB)" << std::endl;
    std::cout << "Output Left Peak: " << std::fixed << std::setprecision(3) << output_left_peak 
              << " (" << output_left_db << " dB)" << std::endl;
    std::cout << "Output Right Peak: " << std::fixed << std::setprecision(3) << output_right_peak 
              << " (" << output_right_db << " dB)" << std::endl;
    std::cout << "Output Average Peak: " << std::fixed << std::setprecision(3) << output_peak_avg 
              << " (" << output_avg_db << " dB)" << std::endl;

    std::cout << "\n=== Gain Analysis ===" << std::endl;
    std::cout << "Input->Output Gain Difference: " << std::fixed << std::setprecision(3) 
              << gain_diff_db << " dB" << std::endl;

    // Analyze results
    const float EXPECTED_GAIN_TOLERANCE_DB = 1.0f; // 1dB tolerance
    bool gain_within_tolerance = std::abs(gain_diff_db) <= EXPECTED_GAIN_TOLERANCE_DB;

    std::cout << "\n=== Test Validation ===" << std::endl;
    std::cout << "Expected: Near unity gain (0 dB) when azimuth == yaw" << std::endl;
    std::cout << "Tolerance: ±" << EXPECTED_GAIN_TOLERANCE_DB << " dB" << std::endl;
    std::cout << "Result: " << (gain_within_tolerance ? "PASS" : "FAIL") << std::endl;

    if (!gain_within_tolerance) {
        std::cout << "\nWARNING: Gain difference exceeds tolerance!" << std::endl;
        std::cout << "This may indicate:" << std::endl;
        std::cout << "  - Pan law compensation in the decode stage" << std::endl;
        std::cout << "  - Unexpected gain behavior in the encode/decode chain" << std::endl;
        std::cout << "  - Need for gain compensation in the signal chain" << std::endl;
    } else {
        std::cout << "\nSUCCESS: Gain is within expected tolerance." << std::endl;
        std::cout << "The encode->decode chain maintains expected gain behavior." << std::endl;
    }

    // Show detailed peak tracker output for reference
    std::cout << "\n=== Detailed Peak Measurements ===" << std::endl;
    peak_tracker_link[0].Print(false, true, true);
    peak_tracker_link[1].Print(false, true, true);
    peak_tracker_link[2].Print(false, true, true);

    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Primary test (azimuth=0°, yaw=0°, diverge=100%, 8ch): " 
              << (gain_within_tolerance ? "PASS" : "FAIL") << std::endl;

    return gain_within_tolerance ? 0 : 1;
} 