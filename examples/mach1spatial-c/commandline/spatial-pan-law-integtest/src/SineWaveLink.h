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

private:
    float m_frequency = 440.0f;

    float m_gain = 1.0f;

private:

    double m_time_per_sample;
};

} // Mach1

#endif //MACH1SPATIALSDK_SINEWAVELINK_H
