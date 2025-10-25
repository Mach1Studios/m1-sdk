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
#include <iomanip>

float amplitudeToDecibels(float amplitude) {
    if (amplitude <= 0.0f) return -100.0f;
    return 20.0f * log10(amplitude);
}

void runSingleMeasurement(int channel_count, float azimuth, float yaw, const std::string& description) {
    std::cout << "\n--- " << description << " ---" << std::endl;
    
    Mach1::RtAudioChain chain{};
    Mach1::PeakTrackerLink peak_tracker_link[3];
    Mach1::SineWaveLink sine_wave_link{};
    Mach1::EncodeLink encode_link{};
    Mach1::DecodeLink decode_link{};
    Mach1::GainLink gain_link{};
    
    // Setup signal chain
    chain.AddLink(&sine_wave_link);
    chain.AddLink(&peak_tracker_link[0]); // Input
    chain.AddLink(&encode_link);
    chain.AddLink(&peak_tracker_link[1]); // Encoded
    chain.AddLink(&decode_link);
    chain.AddLink(&peak_tracker_link[2]); // Output
    chain.AddLink(&gain_link);

    // Configure audio chain
    chain.SetInputChannelCount(1);
    chain.SetOutputChannelCount(2);
    chain.SetAudioBufferInputChannelCount(channel_count);
    chain.SetAudioBufferOutputChannelCount(channel_count);
    chain.SetSampleRate(48000);
    chain.SetBufferSize(512);

    // Configure peak trackers
    peak_tracker_link[0].SetName("Input");
    peak_tracker_link[0].SetOutputChannelCount(1);
    peak_tracker_link[1].SetName("Encoded");
    peak_tracker_link[1].SetOutputChannelCount(channel_count);
    peak_tracker_link[2].SetName("Output");
    peak_tracker_link[2].SetOutputChannelCount(2);

    // Configure sine wave
    sine_wave_link.SetFrequency(1000.0f);
    sine_wave_link.SetGainDecibels(-12.0f);
    sine_wave_link.SetChannelMask(~0);

    // Configure encode
    encode_link.SetInputMode(Mach1EncodeInputMode::Mono);
    switch (channel_count) {
        case 4:
            encode_link.SetOutputMode(M1Spatial_4);
            decode_link.SetDecodeMode(M1DecodeSpatial_4);
            break;
        case 8:
            encode_link.SetOutputMode(M1Spatial_8);
            decode_link.SetDecodeMode(M1DecodeSpatial_8);
            break;
        case 14:
            encode_link.SetOutputMode(M1Spatial_14);
            decode_link.SetDecodeMode(M1DecodeSpatial_14);
            break;
    }

    encode_link.SetAzimuthDegrees(azimuth);
    encode_link.SetElevationDegrees(0.0f);
    encode_link.SetDiverge(1.0f);
    encode_link.SetIsotropicMode(false);
    encode_link.SetEqualPowerMode(true);
    encode_link.SetOrbitRotation(0.0f);
    encode_link.SetStereoSpread(0.5f);
    encode_link.SetAutoOrbit(true);
    encode_link.GeneratePointResults();

    // Configure decode
    decode_link.SetRotationDegrees({yaw, 0.0f, 0.0f});
    decode_link.SetPlatformType(Mach1PlatformDefault);
    decode_link.SetFilterSpeed(0.95f);
    decode_link.SetPointCount(encode_link.GetPointsCount());

    // Mute output
    gain_link.SetGain(0.0f);

    std::cout << "Starting measurement for " << channel_count << "ch, Az=" << azimuth << "°, Yaw=" << yaw << "°..." << std::endl;

    try {
        chain.Start();
        
        // Short measurement period
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        
        // Get measurements quickly before stopping
        float input_peak = peak_tracker_link[0].GetMaxPeak(0);
        float output_left_peak = peak_tracker_link[2].GetMaxPeak(0);
        float output_right_peak = peak_tracker_link[2].GetMaxPeak(1);
        float output_avg_peak = (output_left_peak + output_right_peak) / 2.0f;
        
        float input_db = amplitudeToDecibels(input_peak);
        float output_avg_db = amplitudeToDecibels(output_avg_peak);
        float gain_diff_db = output_avg_db - input_db;
        
        std::cout << "RESULT: " << channel_count << "ch | Az=" << azimuth << "° Yaw=" << yaw << "° | "
                  << "Input=" << std::fixed << std::setprecision(3) << input_peak << " "
                  << "Output=" << output_left_peak << "," << output_right_peak << " | "
                  << "Gain: " << gain_diff_db << " dB" << std::endl;
        
        // Try to stop cleanly, but catch any crash
        std::cout << "Stopping..." << std::endl;
        chain.Stop();
        std::cout << "Stopped successfully." << std::endl;
        
    } catch (...) {
        std::cout << "Exception during processing (measurement may still be valid)" << std::endl;
    }
}

int main(int argc, char *argv[]) {
    std::cout << "=== Quick Real Measurements Test ===" << std::endl;
    std::cout << "Getting actual data from working components" << std::endl;

    // Test the scenarios you mentioned
    runSingleMeasurement(8, 0.0f, 0.0f, "8ch: Az=0°, Yaw=0° (baseline)");
    runSingleMeasurement(4, 0.0f, 0.0f, "4ch: Az=0°, Yaw=0° (compare to 8ch)");
    runSingleMeasurement(14, 0.0f, 0.0f, "14ch: Az=0°, Yaw=0° (expect quieter)");
    
    runSingleMeasurement(8, 45.0f, 45.0f, "8ch: Az=45°, Yaw=45° (matched)");
    runSingleMeasurement(8, 45.0f, 0.0f, "8ch: Az=45°, Yaw=0° (expect +3dB)");
    runSingleMeasurement(4, 45.0f, 0.0f, "4ch: Az=45°, Yaw=0° (expect +3dB)");
    
    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "These are REAL measurements from the Mach1 encode->decode chain." << std::endl;
    std::cout << "Each test ran for 1.5 seconds to get stable peak readings." << std::endl;
    
    return 0;
} 