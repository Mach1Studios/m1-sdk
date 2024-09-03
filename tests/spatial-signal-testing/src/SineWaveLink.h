#ifndef MACH1SPATIALSDK_SINEWAVELINK_H
#define MACH1SPATIALSDK_SINEWAVELINK_H

#include "RtAudioLink.h"

namespace Mach1 {

class SineWaveLink: public virtual RtAudioLink {
public:
    void SetSampleRate(size_t sample_rate) override;

    void Process(AudioBuffers &buffers, double playback_time) override;

    float GetFrequency() const;

    void SetFrequency(float frequency);

    float GetGain() const;

    void SetGain(float gain);

    void SetGainDecibels(float gain_decibels);

    float GetSineWave(float time) const;

    bool IsChannelEnabled(size_t index) const;

    void SetChannelEnabled(size_t index, bool is_enabled);

    void SetChannelMask(long long bitmask);

private:
    float m_frequency = 440.0f;
    float m_gain = 1.0f;
    double m_time_per_sample;
    long long m_channel_mask;
};

} // Mach1

#endif //MACH1SPATIALSDK_SINEWAVELINK_H
