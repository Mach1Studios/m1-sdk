#include "AudioBuffers.h"
#include "SineWaveLink.h"
#include "EncodeLink.h"
#include "DecodeLink.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace Mach1;

float calculateRMS(const std::vector<float>& samples) {
    if (samples.empty()) return 0.0f;
    
    float sumSquares = 0.0f;
    for (float sample : samples) {
        sumSquares += sample * sample;
    }
    return std::sqrt(sumSquares / samples.size());
}

float amplitudeToDecibels(float amplitude) {
    if (amplitude <= 1e-10f) return -100.0f; // Very quiet
    return 20.0f * std::log10(amplitude);
}

class OfflineGainTest {
public:
    struct TestConfiguration {
        float azimuth_degrees = 0.0f;
        float elevation_degrees = 0.0f;
        float diverge = 1.0f; // 100%
        float decode_yaw = 0.0f;
        float decode_pitch = 0.0f;
        float decode_roll = 0.0f;
        int channel_count = 8;
        float test_frequency = 1000.0f;
        float test_duration_seconds = 1.0f;
        float input_gain_db = -12.0f;
        int sample_rate = 48000;
        int buffer_size = 512;
    };

    bool RunTest(const TestConfiguration& config) {
        std::cout << "\n=== Mach1 Offline Encode->Decode Gain Test ===" << std::endl;
        PrintConfiguration(config);

        if (!Setup(config)) {
            std::cerr << "Failed to setup test" << std::endl;
            return false;
        }

        std::cout << "\nProcessing audio buffers..." << std::endl;

        // Calculate number of buffers needed
        int total_samples = static_cast<int>(config.test_duration_seconds * config.sample_rate);
        int num_buffers = (total_samples + config.buffer_size - 1) / config.buffer_size;

        std::vector<float> input_samples;
        std::vector<float> output_left_samples;
        std::vector<float> output_right_samples;

        // Process buffers
        for (int buffer_idx = 0; buffer_idx < num_buffers; buffer_idx++) {
            AudioBuffers buffers(config.buffer_size, 1, config.channel_count);
            
            // Generate input signal
            m_sine_wave.Process(buffers, buffer_idx * config.buffer_size / static_cast<double>(config.sample_rate));
            
            // Store input samples for analysis (sine wave writes to output buffers)
            auto& sine_output = buffers.GetOutputBuffers();
            for (unsigned int i = 0; i < config.buffer_size; i++) {
                input_samples.push_back(sine_output[0][i]);
            }

            // Encode
            m_encode.Process(buffers, 0);
            
            // Decode  
            m_decode.Process(buffers, 0);
            
            // Store output samples for analysis
            auto& output_buffers = buffers.GetOutputBuffers();
            for (unsigned int i = 0; i < config.buffer_size; i++) {
                output_left_samples.push_back(output_buffers[0][i]);
                output_right_samples.push_back(output_buffers[1][i]);
            }
        }

        return AnalyzeResults(input_samples, output_left_samples, output_right_samples);
    }

private:
    SineWaveLink m_sine_wave;
    EncodeLink m_encode;
    DecodeLink m_decode;

    void PrintConfiguration(const TestConfiguration& config) {
        std::cout << "Configuration:" << std::endl;
        std::cout << "  Azimuth: " << config.azimuth_degrees << "°" << std::endl;
        std::cout << "  Elevation: " << config.elevation_degrees << "°" << std::endl;
        std::cout << "  Diverge: " << (config.diverge * 100.0f) << "%" << std::endl;
        std::cout << "  Decode Yaw: " << config.decode_yaw << "°" << std::endl;
        std::cout << "  Decode Pitch: " << config.decode_pitch << "°" << std::endl;
        std::cout << "  Decode Roll: " << config.decode_roll << "°" << std::endl;
        std::cout << "  Spatial Format: " << config.channel_count << " channel" << std::endl;
        std::cout << "  Test Frequency: " << config.test_frequency << " Hz" << std::endl;
        std::cout << "  Input Gain: " << config.input_gain_db << " dB" << std::endl;
        std::cout << "  Sample Rate: " << config.sample_rate << " Hz" << std::endl;
        std::cout << "  Test Duration: " << config.test_duration_seconds << "s" << std::endl;
    }

    bool Setup(const TestConfiguration& config) {
        // Configure sine wave generator
        m_sine_wave.SetSampleRate(config.sample_rate);
        m_sine_wave.SetFrequency(config.test_frequency);
        m_sine_wave.SetGainDecibels(config.input_gain_db);
        m_sine_wave.SetChannelMask(~0); // Fill all channels

        // Configure encode parameters
        m_encode.SetInputMode(Mach1EncodeInputMode::Mono);
        
        switch (config.channel_count) {
            case 4:
                m_encode.SetOutputMode(M1Spatial_4);
                m_decode.SetDecodeMode(M1DecodeSpatial_4);
                break;
            case 8:
                m_encode.SetOutputMode(M1Spatial_8);
                m_decode.SetDecodeMode(M1DecodeSpatial_8);
                break;
            case 14:
                m_encode.SetOutputMode(M1Spatial_14);
                m_decode.SetDecodeMode(M1DecodeSpatial_14);
                break;
            default:
                std::cerr << "Unsupported channel count: " << config.channel_count << std::endl;
                return false;
        }

        m_encode.SetAzimuthDegrees(config.azimuth_degrees);
        m_encode.SetElevationDegrees(config.elevation_degrees);
        m_encode.SetDiverge(config.diverge);
        m_encode.SetIsotropicMode(false);
        m_encode.SetEqualPowerMode(true);
        m_encode.SetOrbitRotation(0.0f);
        m_encode.SetStereoSpread(0.5f);
        m_encode.SetAutoOrbit(true);
        m_encode.GeneratePointResults();

        // Configure decode parameters
        m_decode.SetRotationDegrees({config.decode_yaw, config.decode_pitch, config.decode_roll});
        m_decode.SetPlatformType(Mach1PlatformDefault);
        m_decode.SetFilterSpeed(0.95f);
        m_decode.SetPointCount(m_encode.GetPointsCount());

        return true;
    }

    bool AnalyzeResults(const std::vector<float>& input_samples, 
                       const std::vector<float>& output_left_samples, 
                       const std::vector<float>& output_right_samples) {
        
        // Calculate RMS values
        float input_rms = calculateRMS(input_samples);
        float output_left_rms = calculateRMS(output_left_samples);
        float output_right_rms = calculateRMS(output_right_samples);
        
        // Average the stereo output
        float output_avg_rms = (output_left_rms + output_right_rms) / 2.0f;

        // Convert to dB
        float input_db = amplitudeToDecibels(input_rms);
        float output_left_db = amplitudeToDecibels(output_left_rms);
        float output_right_db = amplitudeToDecibels(output_right_rms);
        float output_avg_db = amplitudeToDecibels(output_avg_rms);

        // Calculate gain difference
        float gain_diff_db = output_avg_db - input_db;

        std::cout << "\n=== Test Results ===" << std::endl;
        std::cout << "Input RMS: " << std::fixed << std::setprecision(6) << input_rms 
                  << " (" << std::setprecision(3) << input_db << " dB)" << std::endl;
        std::cout << "Output Left RMS: " << std::fixed << std::setprecision(6) << output_left_rms 
                  << " (" << std::setprecision(3) << output_left_db << " dB)" << std::endl;
        std::cout << "Output Right RMS: " << std::fixed << std::setprecision(6) << output_right_rms 
                  << " (" << std::setprecision(3) << output_right_db << " dB)" << std::endl;
        std::cout << "Output Average RMS: " << std::fixed << std::setprecision(6) << output_avg_rms 
                  << " (" << std::setprecision(3) << output_avg_db << " dB)" << std::endl;

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
            std::cout << "  - Need for gain compensation" << std::endl;
        } else {
            std::cout << "\nSUCCESS: Gain is within expected tolerance." << std::endl;
            std::cout << "The encode->decode chain maintains expected gain behavior." << std::endl;
        }

        return gain_within_tolerance;
    }
};

int main(int argc, char *argv[]) {
    OfflineGainTest test;

    // Test the specific scenario requested by the user
    OfflineGainTest::TestConfiguration config;
    config.azimuth_degrees = 0.0f;      // Azimuth = 0
    config.elevation_degrees = 0.0f;    // Elevation = 0  
    config.diverge = 1.0f;              // 100% diverge
    config.decode_yaw = 0.0f;           // Yaw = 0 (should match azimuth)
    config.decode_pitch = 0.0f;         // Pitch = 0
    config.decode_roll = 0.0f;          // Roll = 0
    config.channel_count = 8;           // 8-channel Mach1 spatial
    config.test_frequency = 1000.0f;    // 1kHz test tone
    config.test_duration_seconds = 1.0f; // 1 second test
    config.input_gain_db = -12.0f;      // Safe test level

    bool success = test.RunTest(config);

    std::cout << "\n=== Additional Test Scenarios ===" << std::endl;
    
    // Test with different azimuth/yaw combinations
    std::vector<float> test_angles = {0.0f, 45.0f, 90.0f, 180.0f, 270.0f};
    
    for (float angle : test_angles) {
        config.azimuth_degrees = angle;
        config.decode_yaw = angle; // Match azimuth with yaw
        config.test_duration_seconds = 0.5f; // Shorter tests for batch
        
        std::cout << "\n--- Testing Azimuth = Yaw = " << angle << "° ---" << std::endl;
        bool angle_test_success = test.RunTest(config);
        if (!angle_test_success) {
            std::cout << "FAIL: Angle test for " << angle << "° failed" << std::endl;
        }
    }

    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Primary test (azimuth=0, yaw=0, diverge=100%, 8ch): " 
              << (success ? "PASS" : "FAIL") << std::endl;

    if (success) {
        std::cout << "The encode->decode chain maintains expected gain when azimuth matches yaw." << std::endl;
    } else {
        std::cout << "There may be a gain issue in the encode->decode chain that needs investigation." << std::endl;
    }

    return success ? 0 : 1;
} 