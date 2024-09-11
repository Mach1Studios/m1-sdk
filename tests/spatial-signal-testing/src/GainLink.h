#ifndef MACH1SPATIALSDK_GAINLINK_H
#define MACH1SPATIALSDK_GAINLINK_H

#include "RtAudioLink.h"

namespace Mach1 {

class GainLink: public virtual RtAudioLink {
public:
    void SetSampleRate(size_t sample_rate) override;

    void Process(AudioBuffers &buffers, double playback_time) override;

    float GetGain() const;

    void SetGain(float gain);

    void SetGainDecibels(float gain_decibels);

    bool IsChannelEnabled(size_t index) const;

    void SetChannelEnabled(size_t index, bool is_enabled);

    void SetChannelMask(long long bitmask);

private:
    float m_gain = 1.0f;
    double m_time_per_sample;
    long long m_channel_mask;
};

} // Mach1

#endif //MACH1SPATIALSDK_GAINLINK_H
