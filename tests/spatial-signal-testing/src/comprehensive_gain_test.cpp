#include "AudioBuffers.h"
#include "SineWaveLink.h"
#include "EncodeLink.h"
#include "DecodeLink.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace Mach1;

struct TestResult {
    int channel_count;
    float encode_azimuth;
    float decode_yaw;
    float input_peak;
    float output_left_peak;
    float output_right_peak;
    float output_avg_peak;
    float gain_difference_db;
};

class ComprehensiveGainTest {
public:
    ComprehensiveGainTest() {
        // Configure basic settings
        m_sample_rate = 48000;
        m_buffer_size = 512;
        m_test_duration_samples = 48000; // 1 second
        m_test_frequency = 1000.0f;
        m_input_gain_db = -12.0f;
    }

    std::vector<TestResult> RunAllTests() {
        std::vector<TestResult> results;
        
        std::cout << "\n=== Comprehensive Mach1 Gain Analysis ===" << std::endl;
        std::cout << "Testing encode->decode gain across formats and angles" << std::endl;
        
        // Test configurations
        std::vector<int> channel_counts = {4, 8, 14};
        std::vector<float> encode_azimuths = {0.0f, 45.0f};
        std::vector<float> decode_yaws = {0.0f}; // Start with matched
        
        // Test 1: Matched azimuth/yaw pairs
        std::cout << "\n--- Test 1: Matched Encode Azimuth = Decode Yaw ---" << std::endl;
        for (int channels : channel_counts) {
            for (float angle : encode_azimuths) {
                TestResult result = RunSingleTest(channels, angle, angle); // Matched angles
                results.push_back(result);
                PrintSingleResult(result);
            }
        }
        
        // Test 2: Mismatched - encode +45°, decode 0°
        std::cout << "\n--- Test 2: Encode +45°, Decode 0° (Mismatched) ---" << std::endl;
        for (int channels : channel_counts) {
            TestResult result = RunSingleTest(channels, 45.0f, 0.0f); // Mismatched
            results.push_back(result);
            PrintSingleResult(result);
        }
        
        return results;
    }

    void PrintSummaryTable(const std::vector<TestResult>& results) {
        std::cout << "\n=== Summary Table ===" << std::endl;
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "Format | Enc Az | Dec Yaw | Input  | Out L  | Out R  | Avg    | Gain dB" << std::endl;
        std::cout << "-------|--------|---------|--------|--------|--------|--------|--------" << std::endl;
        
        for (const auto& result : results) {
            std::cout << std::setw(5) << result.channel_count << "ch"
                      << " | " << std::setw(5) << result.encode_azimuth << "°"
                      << " | " << std::setw(6) << result.decode_yaw << "°"
                      << " | " << std::setw(6) << result.input_peak
                      << " | " << std::setw(6) << result.output_left_peak
                      << " | " << std::setw(6) << result.output_right_peak
                      << " | " << std::setw(6) << result.output_avg_peak
                      << " | " << std::setw(6) << result.gain_difference_db << " dB"
                      << std::endl;
        }
    }

    void AnalyzePatterns(const std::vector<TestResult>& results) {
        std::cout << "\n=== Pattern Analysis ===" << std::endl;
        
        // Group by scenario
        std::vector<TestResult> matched_0deg, matched_45deg, mismatched_45_0;
        
        for (const auto& result : results) {
            if (result.encode_azimuth == result.decode_yaw) {
                if (result.encode_azimuth == 0.0f) {
                    matched_0deg.push_back(result);
                } else if (result.encode_azimuth == 45.0f) {
                    matched_45deg.push_back(result);
                }
            } else if (result.encode_azimuth == 45.0f && result.decode_yaw == 0.0f) {
                mismatched_45_0.push_back(result);
            }
        }
        
        PrintPatternAnalysis("Matched 0° (Az=0°, Yaw=0°)", matched_0deg);
        PrintPatternAnalysis("Matched 45° (Az=45°, Yaw=45°)", matched_45deg);
        PrintPatternAnalysis("Mismatched (Az=45°, Yaw=0°)", mismatched_45_0);
        
        // Validate user observations
        std::cout << "\n=== Validation of User Observations ===" << std::endl;
        ValidateUserObservations(results);
    }

private:
    int m_sample_rate;
    int m_buffer_size;
    int m_test_duration_samples;
    float m_test_frequency;
    float m_input_gain_db;

    TestResult RunSingleTest(int channel_count, float encode_azimuth, float decode_yaw) {
        TestResult result = {};
        result.channel_count = channel_count;
        result.encode_azimuth = encode_azimuth;
        result.decode_yaw = decode_yaw;
        
        // Setup components
        SineWaveLink sine_wave;
        EncodeLink encode;
        DecodeLink decode;
        
        // Configure sine wave
        sine_wave.SetSampleRate(m_sample_rate);
        sine_wave.SetFrequency(m_test_frequency);
        sine_wave.SetGainDecibels(m_input_gain_db);
        sine_wave.SetChannelMask(~0);
        
        // Configure encode
        encode.SetInputMode(Mach1EncodeInputMode::Mono);
        switch (channel_count) {
            case 4:
                encode.SetOutputMode(M1Spatial_4);
                decode.SetDecodeMode(M1DecodeSpatial_4);
                break;
            case 8:
                encode.SetOutputMode(M1Spatial_8);
                decode.SetDecodeMode(M1DecodeSpatial_8);
                break;
            case 14:
                encode.SetOutputMode(M1Spatial_14);
                decode.SetDecodeMode(M1DecodeSpatial_14);
                break;
        }
        
        encode.SetAzimuthDegrees(encode_azimuth);
        encode.SetElevationDegrees(0.0f);
        encode.SetDiverge(1.0f); // 100%
        encode.SetIsotropicMode(false);
        encode.SetEqualPowerMode(true);
        encode.SetOrbitRotation(0.0f);
        encode.SetStereoSpread(0.5f);
        encode.SetAutoOrbit(true);
        encode.GeneratePointResults();
        
        // Configure decode
        decode.SetRotationDegrees({decode_yaw, 0.0f, 0.0f});
        decode.SetPlatformType(Mach1PlatformDefault);
        decode.SetFilterSpeed(0.95f);
        decode.SetPointCount(encode.GetPointsCount());
        
        // Process audio
        std::vector<float> input_samples;
        std::vector<float> output_left_samples;
        std::vector<float> output_right_samples;
        
        int num_buffers = (m_test_duration_samples + m_buffer_size - 1) / m_buffer_size;
        
        for (int buffer_idx = 0; buffer_idx < num_buffers; buffer_idx++) {
            AudioBuffers buffers(m_buffer_size, 1, channel_count);
            
            // Generate input
            sine_wave.Process(buffers, buffer_idx * m_buffer_size / static_cast<double>(m_sample_rate));
            
            // Store input samples (sine wave writes to output buffers)
            auto& sine_output = buffers.GetOutputBuffers();
            for (unsigned int i = 0; i < m_buffer_size; i++) {
                input_samples.push_back(sine_output[0][i]);
            }
            
            // Encode
            encode.Process(buffers, 0);
            
            // Decode
            decode.Process(buffers, 0);
            
            // Store output samples
            auto& output_buffers = buffers.GetOutputBuffers();
            for (unsigned int i = 0; i < m_buffer_size; i++) {
                output_left_samples.push_back(output_buffers[0][i]);
                output_right_samples.push_back(output_buffers[1][i]);
            }
        }
        
        // Calculate peaks
        result.input_peak = CalculatePeak(input_samples);
        result.output_left_peak = CalculatePeak(output_left_samples);
        result.output_right_peak = CalculatePeak(output_right_samples);
        result.output_avg_peak = (result.output_left_peak + result.output_right_peak) / 2.0f;
        
        // Calculate gain difference
        float input_db = AmplitudeToDecibels(result.input_peak);
        float output_avg_db = AmplitudeToDecibels(result.output_avg_peak);
        result.gain_difference_db = output_avg_db - input_db;
        
        return result;
    }
    
    float CalculatePeak(const std::vector<float>& samples) {
        float peak = 0.0f;
        for (float sample : samples) {
            peak = std::max(peak, std::abs(sample));
        }
        return peak;
    }
    
    float AmplitudeToDecibels(float amplitude) {
        if (amplitude <= 1e-10f) return -100.0f;
        return 20.0f * std::log10(amplitude);
    }
    
    void PrintSingleResult(const TestResult& result) {
        std::cout << std::fixed << std::setprecision(3);
        std::cout << result.channel_count << "ch format | "
                  << "Az=" << result.encode_azimuth << "° "
                  << "Yaw=" << result.decode_yaw << "° | "
                  << "Input=" << result.input_peak << " "
                  << "Output=" << result.output_left_peak << "," << result.output_right_peak << " | "
                  << "Gain: " << result.gain_difference_db << " dB"
                  << std::endl;
    }
    
    void PrintPatternAnalysis(const std::string& scenario, const std::vector<TestResult>& results) {
        if (results.empty()) return;
        
        std::cout << "\n" << scenario << ":" << std::endl;
        for (const auto& result : results) {
            std::cout << "  " << result.channel_count << "ch: " 
                      << std::fixed << std::setprecision(3) << result.gain_difference_db << " dB" << std::endl;
        }
        
        // Check if 4ch and 8ch have similar gains
        if (results.size() >= 2) {
            float gain_4ch = -999.0f, gain_8ch = -999.0f, gain_14ch = -999.0f;
            for (const auto& result : results) {
                if (result.channel_count == 4) gain_4ch = result.gain_difference_db;
                if (result.channel_count == 8) gain_8ch = result.gain_difference_db;
                if (result.channel_count == 14) gain_14ch = result.gain_difference_db;
            }
            
            if (gain_4ch != -999.0f && gain_8ch != -999.0f) {
                float diff_4_8 = std::abs(gain_4ch - gain_8ch);
                std::cout << "  4ch vs 8ch difference: " << std::fixed << std::setprecision(3) 
                          << diff_4_8 << " dB " << (diff_4_8 < 0.5f ? "(Similar ✓)" : "(Different)") << std::endl;
            }
            
            if (gain_14ch != -999.0f && gain_8ch != -999.0f) {
                float diff_8_14 = gain_8ch - gain_14ch;
                std::cout << "  8ch vs 14ch: " << std::fixed << std::setprecision(3) 
                          << diff_8_14 << " dB " << (diff_8_14 > 1.0f ? "(14ch quieter ✓)" : "(Similar levels)") << std::endl;
            }
        }
    }
    
    void ValidateUserObservations(const std::vector<TestResult>& results) {
        // Check: 4ch & 8ch have same gain patterns
        // Check: 14ch is typically quieter
        // Check: Encode +45°/decode 0° adds +3dB for 4ch & 8ch
        
        std::cout << "User observation validation:" << std::endl;
        
        for (const auto& result : results) {
            if (result.encode_azimuth == 45.0f && result.decode_yaw == 0.0f) {
                if (result.channel_count == 4 || result.channel_count == 8) {
                    bool near_3db = std::abs(result.gain_difference_db - 3.0f) < 1.0f;
                    std::cout << "  " << result.channel_count << "ch Az45°/Yaw0°: " 
                              << std::fixed << std::setprecision(3) << result.gain_difference_db 
                              << " dB " << (near_3db ? "(~+3dB ✓)" : "(≠+3dB)") << std::endl;
                }
            }
        }
    }
};

int main(int argc, char *argv[]) {
    ComprehensiveGainTest test;
    
    std::vector<TestResult> results = test.RunAllTests();
    
    test.PrintSummaryTable(results);
    test.AnalyzePatterns(results);
    
    std::cout << "\n=== Test Complete ===" << std::endl;
    std::cout << "This test validates gain behavior across different Mach1 spatial formats." << std::endl;
    
    return 0;
} 