#include "NoiseGenLink.h"
#include "Constants.h"

using namespace Mach1;

NoiseGenLink::NoiseGenLink() : m_distribution(-1.0f, 1.0f), m_channel_mask(~0ll) {

}

float NoiseGenLink::Get() {
    float white_noise = m_distribution(m_rng);

    switch (m_strat) {

        case White:
            return white_noise;
        case Pink:
            m_pink_a = 0.99765f * m_pink_a + white_noise * 0.0990460f;
            m_pink_b = 0.96300f * m_pink_b + white_noise * 0.2965164f;
            m_pink_c = 0.57000f * m_pink_c + white_noise * 1.0526913f;
            return (m_pink_a + m_pink_b + m_pink_c + white_noise * 0.1848f) * 0.25f;
    }

    return white_noise;
}

void NoiseGenLink::Process(AudioBuffers &buffers, double playback_time) {
    auto& out = buffers.GetOutputBuffers();
    for(unsigned int sample_idx = 0; sample_idx < buffers.GetBufferSize(); sample_idx++) {
        float val = m_gain * Get();
        for(unsigned int channel_idx = 0; channel_idx < buffers.GetOutputChannelCount(); channel_idx++) {
            out[channel_idx][sample_idx] = val * (IsChannelEnabled(channel_idx) ? 1.0f : 0.0f);
        }
    }
}

NoiseGenStrategy NoiseGenLink::GetStrategy() const {
    return m_strat;
}

void NoiseGenLink::SetStrategy(NoiseGenStrategy strategy) {
    m_strat = strategy;
}

float NoiseGenLink::GetGain() const {
    return m_gain;
}

void NoiseGenLink::SetGain(float gain) {
    m_gain = gain;
}

void NoiseGenLink::SetGainDecibels(float gain_decibels) {
    SetGain(exp(gain_decibels * MACH1_DB_2_LOG));
}

bool NoiseGenLink::IsChannelEnabled(size_t index) const {
    return m_channel_mask & (1 << index);
}

void NoiseGenLink::SetChannelEnabled(size_t index, bool is_enabled) {
    if (is_enabled) {
        m_channel_mask |= (1 << index);
    } else {
        m_channel_mask &= ~(1 << index);
    }
}

void NoiseGenLink::SetChannelMask(long long int bitmask) {
    m_channel_mask = bitmask;
}
