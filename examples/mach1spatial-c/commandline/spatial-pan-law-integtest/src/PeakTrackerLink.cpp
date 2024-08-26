#include "PeakTrackerLink.h"

#include "Constants.h"

#include <iostream>


using namespace Mach1;

void PeakTrackerLink::Process(AudioBuffers &buffers, double playback_time) {

    auto& out = buffers.GetOutputBuffers();

    for(unsigned int sample_idx = 0; sample_idx < buffers.GetBufferSize(); sample_idx++) {
        for(unsigned int channel_idx = 0; channel_idx< buffers.GetOutputChannelCount(); channel_idx++) {

            float sample = out[channel_idx][sample_idx];
            float curr_peak = m_current_peak[channel_idx];
            float max_peak = m_max_peak[channel_idx];

            if (sample >= curr_peak) {
                curr_peak = sample;
            } else {
                curr_peak *= m_decay_const;
            }

            if (curr_peak > max_peak) {
                max_peak = curr_peak;
            }

            m_current_peak[channel_idx] = curr_peak;
            m_max_peak[channel_idx] = max_peak;
        }
    }

    Print();
}

void PeakTrackerLink::SetPeakDecayTime(double decay_time) {
    m_inv_peak_decay_time = 1.0 / (double)decay_time;
    m_decay_const = std::pow(0.5f, m_inv_peak_decay_time * m_time_per_sample);
}

void PeakTrackerLink::SetSampleRate(size_t sample_rate) {
    m_time_per_sample = 1.0 / (double)sample_rate;
    m_decay_const = std::pow(0.5f, m_inv_peak_decay_time * m_time_per_sample);
}

void PeakTrackerLink::SetOutputChannelCount(unsigned int output_channel_count) {
    m_output_channel_count = output_channel_count;
    m_current_peak.resize(output_channel_count);
    m_max_peak.resize(output_channel_count);
}

float PeakTrackerLink::GetCurrentPeak(unsigned int channel_index) const {
    return m_current_peak[channel_index];
}

float PeakTrackerLink::GetMaxPeak(unsigned int channel_index) const {
    return m_max_peak[channel_index];
}

float PeakTrackerLink::GetCurrentPeakDecibels(unsigned int channel_index) const {
    return logf(GetCurrentPeak(channel_index)) * MACH1_LOG_2_DB;
}

float PeakTrackerLink::GetMaxPeakDecibels(unsigned int channel_index) const {
    return logf(GetMaxPeak(channel_index)) * MACH1_LOG_2_DB;
}

void PeakTrackerLink::Print() const {

    for(unsigned int idx = 0; idx < m_output_channel_count; idx++) {
        std::cout << GetCurrentPeakDecibels(idx) << "db/" << GetMaxPeakDecibels(idx) << "db ";
    }

    std::cout << std::endl;

}
