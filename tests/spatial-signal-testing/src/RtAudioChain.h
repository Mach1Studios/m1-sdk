#ifndef MACH1SPATIALSDK_RTAUDIOCHAIN_H
#define MACH1SPATIALSDK_RTAUDIOCHAIN_H

#include <RtAudio.h>
#include "AudioBuffers.h"
#include "RtAudioLink.h"

namespace Mach1 {

class RtAudioChain {
public:

    ~RtAudioChain();

    RtAudioChain() = default;
    RtAudioChain(const RtAudioChain& other) = delete;
    RtAudioChain& operator=(const RtAudioChain& other) = delete;
    RtAudioChain(RtAudioChain&& other) = delete;
    RtAudioChain& operator=(RtAudioChain&& other) = delete;

    void Start();
    void Stop();

    void SetAudioBufferInputChannelCount(size_t channel_count);
    size_t GetAudioBufferInputChannelCount() const;

    void SetAudioBufferOutputChannelCount(size_t channel_count);
    size_t GetAudioBufferOutputChannelCount() const;

    void SetInputChannelCount(size_t channel_count);
    size_t GetInputChannelCount() const;

    void SetOutputChannelCount(size_t channel_count);
    size_t GetOutputChannelCount() const;

    void SetSampleRate(size_t sample_rate);

    void SetBufferSize(unsigned int buffer_size);

    AudioBuffers* GetBuffers();

    void Process(AudioBuffers& buffers);

    void AddLink(RtAudioLink* link);

private:
    RtAudio m_rt_audio{};

    RtAudio::StreamParameters m_input_parameters{};
    RtAudio::StreamParameters m_output_parameters{};
    unsigned int m_buffer_size{};
    size_t m_sample_rate{};
    AudioBuffers m_buffers{};
    std::vector<RtAudioLink*> m_links{};

    bool m_started = false;
    double m_time_elapsed_per_sample = 0;
    double m_playback_time = 0;
    size_t m_audio_buffer_input_channel_count = 0;
    size_t m_audio_buffer_output_channel_count = 0;
};

} // Mach1

#endif //MACH1SPATIALSDK_RTAUDIOCHAIN_H
