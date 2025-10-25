#include "AudioBuffers.h"
#include "SineWaveLink.h"
#include "EncodeLink.h"
#include "DecodeLink.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <string>

using namespace Mach1;

struct DistributionAnalysis {
    int channel_count;
    float azimuth;
    float yaw;
    std::vector<float> encoded_channel_rms;
    int active_channels;
    float input_rms;
    float output_left_rms;
    float output_right_rms;
    float output_avg_rms;
    float gain_difference_db;
    std::string description;
};

class ChannelDistributionTest {
public:
    ChannelDistributionTest() {
        m_sample_rate = 48000;
        m_buffer_size = 512;
        m_test_duration_samples = 48000; // 1 second
        m_test_frequency = 1000.0f;
        m_input_gain_db = -12.0f;
    }

    std::vector<DistributionAnalysis> RunDistributionTests() {
        std::vector<DistributionAnalysis> results;
        
        std::cout << "\n=== Channel Distribution vs Gain Analysis ===" << std::endl;
        std::cout << "Testing hypothesis: Gain differences relate to encoder->decoder channel distribution" << std::endl;
        std::cout << "Using RMS measurements for proper audio analysis" << std::endl;
        
        // Test scenarios based on user's hypothesis
        results.push_back(RunSingleDistributionTest(8, 0.0f, 0.0f, "8ch: Az=0° (front center, expect 4 active channels)"));
        results.push_back(RunSingleDistributionTest(8, 45.0f, 45.0f, "8ch: Az=45° Yaw=45° (matched, expect 2 active channels)"));
        results.push_back(RunSingleDistributionTest(8, 45.0f, 0.0f, "8ch: Az=45° Yaw=0° (mismatched, expect +3dB)"));
        
        results.push_back(RunSingleDistributionTest(4, 0.0f, 0.0f, "4ch: Az=0° (front center, expect 2 active channels)"));
        results.push_back(RunSingleDistributionTest(4, 45.0f, 45.0f, "4ch: Az=45° Yaw=45° (matched, expect 2 active channels)"));
        results.push_back(RunSingleDistributionTest(4, 45.0f, 0.0f, "4ch: Az=45° Yaw=0° (mismatched, expect +3dB)"));
        
        results.push_back(RunSingleDistributionTest(14, 0.0f, 0.0f, "14ch: Az=0° (front center, expect 1 active channel)"));
        results.push_back(RunSingleDistributionTest(14, 45.0f, 45.0f, "14ch: Az=45° Yaw=45° (matched)"));
        results.push_back(RunSingleDistributionTest(14, 45.0f, 0.0f, "14ch: Az=45° Yaw=0° (mismatched)"));
        
        return results;
    }

    void AnalyzeDistributionPatterns(const std::vector<DistributionAnalysis>& results) {
        std::cout << "\n=== Distribution Pattern Analysis ===" << std::endl;
        std::cout << "Format | Az° | Yaw° | Active Ch | Input RMS | Output RMS | Gain dB | Theory" << std::endl;
        std::cout << "-------|-----|------|-----------|-----------|------------|---------|--------" << std::endl;
        
        for (const auto& result : results) {
            std::string theory = "";
            if (result.azimuth == result.yaw) {
                theory = "Matched";
            } else if (result.azimuth == 45.0f && result.yaw == 0.0f) {
                theory = "Expect +3dB";
            }
            
            std::cout << std::setw(5) << result.channel_count << "ch"
                      << " | " << std::setw(3) << (int)result.azimuth << "°"
                      << " | " << std::setw(4) << (int)result.yaw << "°"
                      << " | " << std::setw(9) << result.active_channels
                      << " | " << std::fixed << std::setprecision(4) << std::setw(9) << result.input_rms
                      << " | " << std::setw(10) << result.output_avg_rms
                      << " | " << std::setprecision(2) << std::setw(7) << result.gain_difference_db
                      << " | " << theory << std::endl;
        }
        
        std::cout << "\n=== Channel Distribution Theory Validation ===" << std::endl;
        ValidateChannelDistributionHypothesis(results);
    }

private:
    int m_sample_rate;
    int m_buffer_size;
    int m_test_duration_samples;
    float m_test_frequency;
    float m_input_gain_db;

    DistributionAnalysis RunSingleDistributionTest(int channel_count, float azimuth, float yaw, const std::string& description) {
        DistributionAnalysis result = {};
        result.channel_count = channel_count;
        result.azimuth = azimuth;
        result.yaw = yaw;
        result.description = description;
        
        std::cout << "\n" << description << std::endl;
        
        // Setup components (no RtAudio to avoid segfaults)
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
        
        encode.SetAzimuthDegrees(azimuth);
        encode.SetElevationDegrees(0.0f);
        encode.SetDiverge(1.0f); // 100%
        encode.SetIsotropicMode(false);
        encode.SetEqualPowerMode(true);
        encode.SetOrbitRotation(0.0f);
        encode.SetStereoSpread(0.5f);
        encode.SetAutoOrbit(true);
        encode.GeneratePointResults();
        
        // Configure decode
        decode.SetRotationDegrees({yaw, 0.0f, 0.0f});
        decode.SetPlatformType(Mach1PlatformDefault);
        decode.SetFilterSpeed(0.95f);
        decode.SetPointCount(encode.GetPointsCount());
        
        // Process audio and collect samples
        std::vector<float> input_samples;
        std::vector<float> output_left_samples;
        std::vector<float> output_right_samples;
        std::vector<std::vector<float>> encoded_channel_samples(channel_count);
        
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
            
            // Store encoded channel samples for distribution analysis
            auto& encoded_buffers = buffers.GetOutputBuffers();
            for (int ch = 0; ch < channel_count; ch++) {
                for (unsigned int i = 0; i < m_buffer_size; i++) {
                    encoded_channel_samples[ch].push_back(encoded_buffers[ch][i]);
                }
            }
            
            // Decode
            decode.Process(buffers, 0);
            
            // Store output samples
            for (unsigned int i = 0; i < m_buffer_size; i++) {
                output_left_samples.push_back(encoded_buffers[0][i]);
                output_right_samples.push_back(encoded_buffers[1][i]);
            }
        }
        
        // Calculate RMS values
        result.input_rms = CalculateRMS(input_samples);
        result.output_left_rms = CalculateRMS(output_left_samples);
        result.output_right_rms = CalculateRMS(output_right_samples);
        result.output_avg_rms = (result.output_left_rms + result.output_right_rms) / 2.0f;
        
        // Analyze encoded channel distribution
        result.encoded_channel_rms.resize(channel_count);
        result.active_channels = 0;
        for (int ch = 0; ch < channel_count; ch++) {
            result.encoded_channel_rms[ch] = CalculateRMS(encoded_channel_samples[ch]);
            if (result.encoded_channel_rms[ch] > 0.001f) { // Threshold for "active" channel
                result.active_channels++;
            }
        }
        
        // Calculate gain difference
        float input_db = AmplitudeToDecibels(result.input_rms);
        float output_avg_db = AmplitudeToDecibels(result.output_avg_rms);
        result.gain_difference_db = output_avg_db - input_db;
        
        // Print detailed channel distribution
        PrintChannelDistribution(result);
        
        return result;
    }
    
    float CalculateRMS(const std::vector<float>& samples) {
        if (samples.empty()) return 0.0f;
        
        double sum_squares = 0.0;
        for (float sample : samples) {
            sum_squares += sample * sample;
        }
        return static_cast<float>(std::sqrt(sum_squares / samples.size()));
    }
    
    float AmplitudeToDecibels(float amplitude) {
        if (amplitude <= 1e-10f) return -100.0f;
        return 20.0f * std::log10(amplitude);
    }
    
    void PrintChannelDistribution(const DistributionAnalysis& result) {
        std::cout << "  Encoded channel RMS: [";
        for (int ch = 0; ch < result.channel_count; ch++) {
            std::cout << std::fixed << std::setprecision(3) << result.encoded_channel_rms[ch];
            if (ch < result.channel_count - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        std::cout << "  Active channels: " << result.active_channels << std::endl;
        std::cout << "  Input RMS: " << std::fixed << std::setprecision(4) << result.input_rms 
                  << " (" << AmplitudeToDecibels(result.input_rms) << " dB)" << std::endl;
        std::cout << "  Output avg RMS: " << result.output_avg_rms 
                  << " (" << AmplitudeToDecibels(result.output_avg_rms) << " dB)" << std::endl;
        std::cout << "  Gain difference: " << std::setprecision(2) << result.gain_difference_db << " dB" << std::endl;
    }
    
    void ValidateChannelDistributionHypothesis(const std::vector<DistributionAnalysis>& results) {
        std::cout << "Hypothesis: +3dB gain occurs when encoding distributes to fewer channels than expected" << std::endl;
        std::cout << std::endl;
        
        for (const auto& result : results) {
            if (result.azimuth == 45.0f && result.yaw == 0.0f) {
                bool has_3db_boost = std::abs(result.gain_difference_db - 3.0f) < 1.0f;
                std::cout << result.channel_count << "ch Az=45° Yaw=0°: " 
                          << result.active_channels << " active channels, "
                          << std::fixed << std::setprecision(2) << result.gain_difference_db << " dB gain "
                          << (has_3db_boost ? "✓" : "✗") << std::endl;
            }
        }
        
        std::cout << std::endl;
        std::cout << "Key insight: When encoder distributes to fewer channels (2 at 45°) vs more channels (4 at 0°)," << std::endl;
        std::cout << "the decoder may concentrate energy differently, leading to gain changes." << std::endl;
    }
};

int main(int argc, char *argv[]) {
    ChannelDistributionTest test;
    
    std::vector<DistributionAnalysis> results = test.RunDistributionTests();
    
    test.AnalyzeDistributionPatterns(results);
    
    std::cout << "\n=== Audio Measurement Summary ===" << std::endl;
    std::cout << "✓ Using RMS measurements (proper for audio analysis)" << std::endl;
    std::cout << "✓ No segfaults (offline processing)" << std::endl;
    std::cout << "✓ Channel distribution analysis included" << std::endl;
    std::cout << "✓ Testing gain normalization hypothesis" << std::endl;
    
    return 0;
} 