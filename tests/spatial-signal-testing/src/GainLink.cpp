#include "GainLink.h"

#include "Constants.h"

using namespace Mach1;

void GainLink::SetSampleRate(size_t sample_rate) {
    m_time_per_sample = 1.0 / (double) sample_rate;
}

void GainLink::Process(AudioBuffers &buffers, double playback_time) {

    auto& out = buffers.GetOutputBuffers();
    for(unsigned int sample_idx = 0; sample_idx < buffers.GetBufferSize(); sample_idx++) {
        for(unsigned int channel_idx = 0; channel_idx < buffers.GetOutputChannelCount(); channel_idx++) {
            out[channel_idx][sample_idx] *= m_gain;
        }
    }

}

float GainLink::GetGain() const {
    return m_gain;
}

void GainLink::SetGain(float gain) {
    m_gain = gain;
}

void GainLink::SetGainDecibels(float gain_decibels) {
    SetGain(exp(gain_decibels * MACH1_DB_2_LOG));
}
