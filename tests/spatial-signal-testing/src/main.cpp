#include "RtAudioChain.h"
#include "SineWaveLink.h"
#include "PeakTrackerLink.h"
#include "EncodeLink.h"
#include "DecodeLink.h"
#include "TranscodeLink.h"
#include "NoiseGenLink.h"

#include <iostream>
#include <memory>

#define M1_FORMAT_CHANNEL_COUNT 14

int main(int argc, char *argv[]) {

    Mach1::RtAudioChain chain{};
    Mach1::PeakTrackerLink peak_tracker_link[4];
    Mach1::SineWaveLink sine_wave_link{};
    Mach1::NoiseGenLink noise_gen_link{};
    Mach1::EncodeLink encode_link{};
//    Mach1::TranscodeLink transode_link{};
    Mach1::DecodeLink decode_link{};

    chain.AddLink(&sine_wave_link);
    chain.AddLink(&peak_tracker_link[0]);
    chain.AddLink(&encode_link);
    chain.AddLink(&peak_tracker_link[1]);
//    chain.AddLink(&transode_link);
//    chain.AddLink(&peak_tracker_link[2]);
    chain.AddLink(&decode_link);
    chain.AddLink(&peak_tracker_link[3]);

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
//    peak_tracker_link[2].SetName("Transcoded");
    peak_tracker_link[3].SetName("Decoded");
    peak_tracker_link[3].SetOutputChannelCount(2); // limit to number of output channels

    // We copy the generators to each channel to make it easy for Mach1Encode to apply gain coeffs
    sine_wave_link.SetFrequency(1000);
    sine_wave_link.SetGainDecibels(0);
    sine_wave_link.SetChannelMask(~0); // flip all bits to 1, fill all channels
    //sine_wave_link.SetChannelEnabled(0, true); // enable only first channel

    noise_gen_link.SetStrategy(White);
    noise_gen_link.SetGainDecibels(0);
    //noise_gen_link.SetChannelEnabled(0, true); // enable only first channel

    if (M1_FORMAT_CHANNEL_COUNT == 4) {
        encode_link.SetOutputMode(Mach1EncodeOutputModeM1Spatial_4);
        decode_link.SetDecodeAlgoType(Mach1DecodeAlgoSpatial_4);
    } else if (M1_FORMAT_CHANNEL_COUNT == 8) {
        encode_link.SetOutputMode(Mach1EncodeOutputModeM1Spatial_8);
        decode_link.SetDecodeAlgoType(Mach1DecodeAlgoSpatial_8);
    } else if (M1_FORMAT_CHANNEL_COUNT == 12) {
        encode_link.SetOutputMode(Mach1EncodeOutputModeM1Spatial_12);
        decode_link.SetDecodeAlgoType(Mach1DecodeAlgoSpatial_12);
    } else if (M1_FORMAT_CHANNEL_COUNT == 14) {
        encode_link.SetOutputMode(Mach1EncodeOutputModeM1Spatial_14);
        decode_link.SetDecodeAlgoType(Mach1DecodeAlgoSpatial_14);
    } else {
        // default Mach1Spatial_8
        encode_link.SetOutputMode(Mach1EncodeOutputModeM1Spatial_8);
        decode_link.SetDecodeAlgoType(Mach1DecodeAlgoSpatial_8);
    }

    encode_link.SetInputMode(Mach1EncodeInputModeType::Mach1EncodeInputModeMono);
    encode_link.SetAzimuthDegrees(0);
    encode_link.SetElevation(0);
    encode_link.SetDiverge(1.0);
    encode_link.SetIsotropicMode(false);
    encode_link.SetEqualPowerMode(false);
    encode_link.SetOutputGain(4.0, false);
    encode_link.SetOrbitRotation(0.0);
    encode_link.SetStereoSpread(0.5);
    encode_link.SetAutoOrbit(true);
    encode_link.GeneratePointResults();

    decode_link.SetRotationDegrees({0, 0, 0}); // ypr
    decode_link.SetPlatformType(Mach1PlatformDefault);
    decode_link.SetFilterSpeed(1.0);
    decode_link.SetPointCount(encode_link.GetPointsCount());

    chain.Start();

    char input;
    std::cout << "Press anything to exit." << std::endl;
    std::cin.get(input);
    chain.Stop();

    return 0;
}
