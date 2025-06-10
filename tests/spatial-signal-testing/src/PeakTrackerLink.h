#ifndef MACH1SPATIALSDK_PEAKTRACKERLINK_H
#define MACH1SPATIALSDK_PEAKTRACKERLINK_H

#include "RtAudioLink.h"

#include <string>

namespace Mach1 {

class PeakTrackerLink : public virtual RtAudioLink {
public:

    PeakTrackerLink();

    void SetSampleRate(size_t sample_rate) override;

    void Process(AudioBuffers &buffers, double playback_time) override;

    void SetPeakDecayTime(double decay_time);

    void SetOutputChannelCount(unsigned int output_channel_count) override;

    float GetCurrentPeak(unsigned int channel_index) const;

    float GetMaxPeak(unsigned int channel_index) const;

    float GetCurrentPeakDecibels(unsigned int channel_index) const;

    float GetMaxPeakDecibels(unsigned int channel_index) const;

    void Print(bool show_current_peaks, bool show_max_peaks, bool show_values_as_dB) const;

    void SetName(const std::string &name);

private:
    double m_time_per_sample;
    double m_decay_const;
    double m_inv_peak_decay_time;

    float m_max_value = 10000000;
    float m_min_value = -10000000;

    std::vector<float> m_current_peak;
    std::vector<float> m_max_peak;
    unsigned int m_output_channel_count;

    std::string m_name;

};

} // Mach1

#endif //MACH1SPATIALSDK_PEAKTRACKERLINK_H
