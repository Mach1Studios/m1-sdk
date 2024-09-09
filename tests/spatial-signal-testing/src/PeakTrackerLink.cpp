#include "PeakTrackerLink.h"

#include "Constants.h"

#include <iostream>
#include <cfloat>
#include <algorithm>
#include <iomanip>

using namespace Mach1;


PeakTrackerLink::PeakTrackerLink() {
    SetPeakDecayTime(0.1);
}

void PeakTrackerLink::Process(AudioBuffers &buffers, double playback_time) {

    auto &out = buffers.GetOutputBuffers();

    for (unsigned int sample_idx = 0; sample_idx < buffers.GetBufferSize(); sample_idx++) {
        for (unsigned int channel_idx = 0; channel_idx < buffers.GetOutputChannelCount(); channel_idx++) {

            float sample = std::abs(out[channel_idx][sample_idx]);
            float curr_peak = m_current_peak[channel_idx];
            float max_peak = m_max_peak[channel_idx];

            if (sample > curr_peak) {
                curr_peak = sample;
//                curr_peak = std::min(sample, m_max_value);
            } else {
                curr_peak = curr_peak * (float) m_decay_const;
//                curr_peak = std::max(curr_peak * (float)m_decay_const, m_min_value);
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
    m_inv_peak_decay_time = 1.0 / (double) decay_time;
    m_decay_const = std::pow(0.5f, m_inv_peak_decay_time * m_time_per_sample);
}

void PeakTrackerLink::SetSampleRate(size_t sample_rate) {
    m_time_per_sample = 1.0 / (double) sample_rate;
    m_decay_const = std::pow(0.5f, m_inv_peak_decay_time * m_time_per_sample);
}

void PeakTrackerLink::SetOutputChannelCount(unsigned int output_channel_count) {
    m_output_channel_count = output_channel_count;
    m_current_peak.resize(output_channel_count);
    m_max_peak.resize(output_channel_count);

    for (unsigned int idx = 0; idx < output_channel_count; idx++) {
        m_current_peak[idx] = FLT_MIN;
        m_max_peak[idx] = FLT_MIN;
    }
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

    std::cout << m_name << " (dB)";
    std::cout << std::endl;

    // current peak dB per channel
    std::cout << "\t";
    for (unsigned int idx = 0; idx < m_current_peak.size(); idx++) {
        std::cout << std::fixed << std::setprecision(3) << GetCurrentPeakDecibels(idx) << ", ";
    }
    std::cout << std::endl;

    // max peak dB per channel
    std::cout << "\t";
    for (unsigned int idx = 0; idx < m_current_peak.size(); idx++) {
        std::cout << std::fixed << std::setprecision(3) << GetMaxPeakDecibels(idx) << ", ";
    }
    std::cout << std::endl;

    float sum = 0.0;
    for (unsigned int idx = 0; idx < m_current_peak.size(); idx++) {
        sum += GetMaxPeak(idx);
    }

    std::cout << std::fixed << std::setprecision(3) << "\tSum: " << sum;
    std::cout << std::endl;
}

void PeakTrackerLink::SetName(const std::string &name) {
    m_name = name;
}
