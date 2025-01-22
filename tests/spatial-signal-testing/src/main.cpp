#include "RtAudioChain.h"
#include "SineWaveLink.h"
#include "PeakTrackerLink.h"
#include "GainLink.h"
#include "EncodeLink.h"
#include "DecodeLink.h"
#include "TranscodeLink.h"
#include "NoiseGenLink.h"

#include <iostream>
#include <memory>

float calculateRMS(const float* samples, int numSamples) {
    float sumSquares = 0.0f;
    for (int i = 0; i < numSamples; i++) {
        sumSquares += samples[i] * samples[i];
    }
    return sqrt(sumSquares / numSamples);
}

int main(int argc, char *argv[]) {

    Mach1::RtAudioChain chain{};
    Mach1::PeakTrackerLink peak_tracker_link[4];
    Mach1::SineWaveLink sine_wave_link{};
    Mach1::NoiseGenLink noise_gen_link{};
    Mach1::EncodeLink encode_link{};
    Mach1::TranscodeLink transode_link{};
    Mach1::DecodeLink decode_link{};
    Mach1::GainLink gain_link[3];
    
    chain.AddLink(&sine_wave_link);
    chain.AddLink(&peak_tracker_link[0]);
    chain.AddLink(&encode_link);
    chain.AddLink(&peak_tracker_link[1]);
//    chain.AddLink(&transode_link);
//    chain.AddLink(&peak_tracker_link[2]);
    chain.AddLink(&gain_link[0]); // Gain pre-Decode
    chain.AddLink(&decode_link);
    chain.AddLink(&gain_link[1]); // Gain post-Decode
    chain.AddLink(&peak_tracker_link[3]);
    chain.AddLink(&gain_link[2]); // Output Gain

    #define M1_FORMAT_CHANNEL_COUNT 4
    float diverge = 1;
    float azimuth = 45;
    float elevation = 0;
    //float enc_mult = (diverge > 0.0) ? M1_FORMAT_CHANNEL_COUNT * diverge : 1.0;
    float enc_mult = 1.0;
    float dec_mult = M1_FORMAT_CHANNEL_COUNT/2;
    encode_link.SetOutputGain(enc_mult, false);
    gain_link[0].SetGain(dec_mult); // pre-Decode
    gain_link[1].SetGain(1); // post-Decode for stereo
    
    chain.SetInputChannelCount(1);
    chain.SetOutputChannelCount(2);
    chain.SetAudioBufferInputChannelCount(M1_FORMAT_CHANNEL_COUNT);
    chain.SetAudioBufferOutputChannelCount(M1_FORMAT_CHANNEL_COUNT);
    chain.SetSampleRate(48000);
    chain.SetBufferSize(512);

    peak_tracker_link[0].SetName("Clean");
    peak_tracker_link[0].SetOutputChannelCount(M1_FORMAT_CHANNEL_COUNT);
    peak_tracker_link[1].SetName("Encoded");
    peak_tracker_link[1].SetOutputChannelCount(M1_FORMAT_CHANNEL_COUNT);
    peak_tracker_link[2].SetName("Transcoded");
    peak_tracker_link[2].SetOutputChannelCount(M1_FORMAT_CHANNEL_COUNT);
    peak_tracker_link[3].SetName("Decoded");
    peak_tracker_link[3].SetOutputChannelCount(2); // limit to number of output channels, otherwise the peak sum accounts for channels not outputting

    // We copy the generators to each channel to make it easy for Mach1Encode to apply gain coeffs
    sine_wave_link.SetFrequency(1000);
    sine_wave_link.SetGainDecibels(0);
    sine_wave_link.SetChannelMask(~0); // flip all bits to 1, fill all channels
    //sine_wave_link.SetChannelEnabled(0, true); // enable only first channel

    noise_gen_link.SetStrategy(White);
    noise_gen_link.SetGainDecibels(0);
    //noise_gen_link.SetChannelEnabled(0, true); // enable only first channel

    if (M1_FORMAT_CHANNEL_COUNT == 4) {
        encode_link.SetOutputMode(M1Spatial_4);
        decode_link.SetDecodeMode(M1DecodeSpatial_4);
    } else if (M1_FORMAT_CHANNEL_COUNT == 8) {
        encode_link.SetOutputMode(M1Spatial_8);
        decode_link.SetDecodeMode(M1DecodeSpatial_8);
    } else if (M1_FORMAT_CHANNEL_COUNT == 14) {
        encode_link.SetOutputMode(M1Spatial_14);
        decode_link.SetDecodeMode(M1DecodeSpatial_14);
    } else {
        // default Mach1Spatial_8
        encode_link.SetOutputMode(M1Spatial_8);
        decode_link.SetDecodeMode(M1DecodeSpatial_8);
    }

    encode_link.SetInputMode(Mach1EncodeInputMode::Mono);
    encode_link.SetAzimuthDegrees(azimuth);
    encode_link.SetElevation(elevation);
    encode_link.SetDiverge(diverge);
    encode_link.SetIsotropicMode(false);
    encode_link.SetEqualPowerMode(true);
    encode_link.SetOrbitRotation(0.0);
    encode_link.SetStereoSpread(0.5);
    encode_link.SetAutoOrbit(true);
    encode_link.GeneratePointResults();

//    transode_link.SetInputFormat(M1_FORMAT_CHANNEL_COUNT);
//    transode_link.SetOutputFormat(M1_FORMAT_CHANNEL_COUNT);
//    transode_link.ProcessConversionPath();

    decode_link.SetRotationDegrees({0, 0, 0}); // ypr
    decode_link.SetPlatformType(Mach1PlatformDefault);
    decode_link.SetFilterSpeed(0.95);
    decode_link.SetPointCount(encode_link.GetPointsCount());

    gain_link[2].SetGain(0.0); // use this to mute the output if needed

    chain.Start();

    char input;
    std::cout << "Press anything to exit." << std::endl;
    std::cin.get(input);
    chain.Stop();

    return 0;
}
