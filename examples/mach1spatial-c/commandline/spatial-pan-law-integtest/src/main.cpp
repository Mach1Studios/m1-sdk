#include "RtAudioChain.h"
#include "SineWaveLink.h"
#include "PeakTrackerLink.h"
#include "EncodeLink.h"
#include "DecodeLink.h"

#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {

    Mach1::RtAudioChain chain{};
    Mach1::SineWaveLink sine_wave_link{};
    Mach1::PeakTrackerLink peak_tracker_link[3];
    Mach1::EncodeLink encode_link{};
    Mach1::DecodeLink decode_link{};

    chain.SetInputChannelCount(2);
    chain.SetOutputChannelCount(2);
    chain.SetSampleRate(48000);
    chain.SetBufferSize(256);

    sine_wave_link.SetFrequency(440);
    sine_wave_link.SetGainDecibels(5.0);

    chain.AddLink(&sine_wave_link);
    chain.AddLink(&peak_tracker_link[0]);
    chain.AddLink(&encode_link);
    chain.AddLink(&peak_tracker_link[1]);
    chain.AddLink(&decode_link);
    chain.AddLink(&peak_tracker_link[2]);

    chain.Start();

    char input;
    std::cout << "Press anything to exit." << std::endl;
    std::cin.get(input);
    chain.Stop();

    return 0;
}
