#include "RtAudioChain.h"
#include "SineWaveLink.h"
#include "PeakTrackerLink.h"
#include "EncodeLink.h"
#include "DecodeLink.h"
#include "TranscodeLink.h"
#include "NoiseGenLink.h"

#include <iostream>
#include <memory>

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

    chain.SetInputChannelCount(2);
    chain.SetOutputChannelCount(2);
    chain.SetAudioBufferInputChannelCount(14);
    chain.SetAudioBufferOutputChannelCount(14);
    chain.SetSampleRate(48000);
    chain.SetBufferSize(512);

    peak_tracker_link[0].SetName("Clean");
    peak_tracker_link[1].SetName("Encoded");
//    peak_tracker_link[2].SetName("Transcoded");
    peak_tracker_link[3].SetName("Decoded");

    sine_wave_link.SetFrequency(1000);
    sine_wave_link.SetGainDecibels(0);
    sine_wave_link.SetChannelMask(1); // disable all channels except first

    noise_gen_link.SetStrategy(White);
    noise_gen_link.SetGainDecibels(0);
    noise_gen_link.SetChannelMask(0); // disable all channels
    noise_gen_link.SetChannelEnabled(0, true); // enable only first channel

    encode_link.SetInputMode(Mach1EncodeInputModeType::Mach1EncodeInputModeMono);
    encode_link.SetOutputMode(Mach1EncodeOutputModeType::Mach1EncodeOutputModeM1Spatial_14);
    encode_link.SetAzimuth(0);
    encode_link.SetElevation(0);
    encode_link.SetIsotropicMode(false);
    encode_link.SetEqualPowerMode(false);
    encode_link.SetOutputGain(0.0, true);
    encode_link.SetOrbitRotation(0.0);
    encode_link.SetDiverge(0.5);
    encode_link.SetStereoSpread(0.5);
    encode_link.SetAutoOrbit(true);

    decode_link.SetRotationDegrees({0, 0, 0});
    decode_link.SetDecodeAlgoType(Mach1DecodeAlgoSpatial_14);
    decode_link.SetPlatformType(Mach1PlatformDefault);
    decode_link.SetFilterSpeed(1.0);

    encode_link.GeneratePointResults();
    decode_link.SetPointCount(encode_link.GetPointsCount());

    chain.Start();

    char input;
    std::cout << "Press anything to exit." << std::endl;
    std::cin.get(input);
    chain.Stop();

    return 0;
}
