#ifndef MACH1SPATIALSDK_NOISEGENLINK_H
#define MACH1SPATIALSDK_NOISEGENLINK_H

#include "RtAudioLink.h"
#include "NoiseGenStrategy.h"

#include <random>

namespace Mach1 {

class NoiseGenLink : public virtual RtAudioLink {
public:

    NoiseGenLink();

    void Process(AudioBuffers &buffers, double playback_time) override;

    NoiseGenStrategy GetStrategy() const;

    void SetStrategy(NoiseGenStrategy strategy);

    float GetGain() const;

    void SetGain(float gain);

    void SetGainDecibels(float gain_decibels);

    float Get();

    bool IsChannelEnabled(size_t index) const;

    void SetChannelEnabled(size_t index, bool is_enabled);

    void SetChannelMask(long long bitmask);

private:

    long long m_channel_mask;

    float m_gain = 1.0f;

    float m_pink_a = 0;
    float m_pink_b = 0;
    float m_pink_c = 0;

    NoiseGenStrategy m_strat;
    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_distribution;
};

} // Mach1

#endif //MACH1SPATIALSDK_NOISEGENLINK_H
