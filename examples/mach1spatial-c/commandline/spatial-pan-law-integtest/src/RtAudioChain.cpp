#include "RtAudioChain.h"
#include "AudioBuffers.h"

using namespace Mach1;


int ProcessBuffers(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime,
                   RtAudioStreamStatus status, void *userData)
{
    auto* chain = static_cast<RtAudioChain*>(userData);
    unsigned int in_count = chain->GetInputChannelCount();
    unsigned int out_count = chain->GetOutputChannelCount();

    switch(status) {
        case RTAUDIO_INPUT_OVERFLOW:
            std::cout << "Input overflow" << std::endl;
            break;
        case RTAUDIO_OUTPUT_UNDERFLOW:
            std::cout << "Output underflow" << std::endl;
            break;
        default:
            break;
    }

    AudioBuffers* buffs = chain->GetBuffers();
    buffs->Restructure(nFrames, in_count, out_count);

    buffs->EmplaceIntoOutput((float*)outputBuffer, nFrames, out_count);
    buffs->EmplaceIntoInput((float*)inputBuffer, nFrames, in_count);
    chain->Process(*buffs);
    buffs->DumpOutputIntoBuffer((float*)outputBuffer, nFrames, out_count);
    buffs->DumpInputIntoBuffer((float*)inputBuffer, nFrames, in_count);

    return 0;
}

RtAudioChain::~RtAudioChain() {
    Stop();
}

void RtAudioChain::Start() {
    if (m_started) return;

    m_input_parameters.deviceId = m_rt_audio.getDefaultInputDevice();
    m_output_parameters.deviceId = m_rt_audio.getDefaultOutputDevice();

    RtAudio::StreamOptions options;
    options.streamName = "Mach1 Spatial Pan Law Integration Test";

    options.flags = RTAUDIO_SCHEDULE_REALTIME;

    m_rt_audio.openStream(&m_output_parameters, NULL,
                          RTAUDIO_FLOAT32, m_sample_rate, &m_buffer_size,
                          ProcessBuffers, this, &options);

    m_rt_audio.startStream();

    m_started = true;
}

void RtAudioChain::Stop() {
    if (!m_started) return;
    m_rt_audio.stopStream();
    m_started = false;
}

void RtAudioChain::Process(AudioBuffers &buffers) {
    for(auto* l: m_links) {
        l->Process(buffers, m_playback_time);
    }

    m_playback_time += buffers.GetBufferSize() * m_time_elapsed_per_sample;
}

void RtAudioChain::AddLink(RtAudioLink* link) {
    link->SetSampleRate(m_sample_rate);
    link->SetOutputChannelCount(m_output_parameters.nChannels);
    m_links.push_back(link);
}

// =====================================================================================================================
// ============================================= SETTERS/GETTERS =======================================================
// =====================================================================================================================

void RtAudioChain::SetInputChannelCount(size_t channel_count) {
    m_input_parameters.nChannels = channel_count;
}

size_t RtAudioChain::GetInputChannelCount() const {
    return m_input_parameters.nChannels;
}

void RtAudioChain::SetOutputChannelCount(size_t channel_count) {
    m_output_parameters.nChannels = channel_count;
}

size_t RtAudioChain::GetOutputChannelCount() const {
    return m_output_parameters.nChannels;
}

void RtAudioChain::SetSampleRate(size_t sample_rate) {
    m_sample_rate = sample_rate;
    m_time_elapsed_per_sample = 1.0 / (double) sample_rate;

    for(auto* l: m_links) {
        l->SetSampleRate(sample_rate);
    }
}

void RtAudioChain::SetBufferSize(unsigned int buffer_size) {
    m_buffer_size = buffer_size;
}

AudioBuffers *RtAudioChain::GetBuffers() {
    return &m_buffers;
}
