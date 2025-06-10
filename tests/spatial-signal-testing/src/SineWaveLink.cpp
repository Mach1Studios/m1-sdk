#include "SineWaveLink.h"

#include "Constants.h"

using namespace Mach1;

void SineWaveLink::SetSampleRate(size_t sample_rate) {
    m_time_per_sample = 1.0 / (double) sample_rate;
}

void SineWaveLink::Process(AudioBuffers &buffers, double playback_time) {

    auto& out = buffers.GetOutputBuffers();
    for(unsigned int sample_idx = 0; sample_idx < buffers.GetBufferSize(); sample_idx++) {
        double t = playback_time + (double) sample_idx * m_time_per_sample;
        float val = m_gain * GetSineWave(MACH1_TAU * m_frequency * t);
        for(unsigned int channel_idx = 0; channel_idx < buffers.GetOutputChannelCount(); channel_idx++) {
            out[channel_idx][sample_idx] = val * (IsChannelEnabled(channel_idx) ? 1.0f : 0.0f);
        }
    }

}

float SineWaveLink::GetFrequency() const {
    return m_frequency;
}

void SineWaveLink::SetFrequency(float frequency) {
    m_frequency = frequency;
}

float SineWaveLink::GetSineWave(float time) const {
    return sin(fmod(time, MACH1_TAU));
}

float SineWaveLink::GetGain() const {
    return m_gain;
}

void SineWaveLink::SetGain(float gain) {
    m_gain = gain;
}

void SineWaveLink::SetGainDecibels(float gain_decibels) {
    SetGain(exp(gain_decibels * MACH1_DB_2_LOG));
}


bool SineWaveLink::IsChannelEnabled(size_t index) const {
    return m_channel_mask & (1 << index);
}

void SineWaveLink::SetChannelEnabled(size_t index, bool is_enabled) {
    if (is_enabled) {
        m_channel_mask |= (1 << index);
    } else {
        m_channel_mask &= ~(1 << index);
    }
}

void SineWaveLink::SetChannelMask(long long int bitmask) {
    m_channel_mask = bitmask;
}
