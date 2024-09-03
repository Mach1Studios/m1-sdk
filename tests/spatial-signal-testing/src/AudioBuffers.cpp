#include "AudioBuffers.h"

using namespace Mach1;

AudioBuffers::AudioBuffers(unsigned int buffer_size, unsigned int input_channel_count,
                           unsigned int output_channel_count)
{
    Restructure(buffer_size, input_channel_count, output_channel_count);
}

unsigned int AudioBuffers::GetBufferSize() const {
    return m_buffer_size;
}

unsigned int AudioBuffers::GetInputChannelCount() const {
    return m_input_channel_count;
}

unsigned int AudioBuffers::GetOutputChannelCount() const {
    return m_output_channel_count;
}

void AudioBuffers::Restructure(unsigned int buffer_size, unsigned int input_channel_count,
                               unsigned int output_channel_count)
{
    m_input_buffers.resize(input_channel_count);
    for (int idx = 0; idx < input_channel_count; idx++) m_input_buffers[idx].resize(buffer_size);

    m_output_buffers.resize(output_channel_count);
    for (int idx = 0; idx < input_channel_count; idx++) m_output_buffers[idx].resize(buffer_size);

    m_buffer_size = buffer_size;
    m_input_channel_count = input_channel_count;
    m_output_channel_count = output_channel_count;
}

std::vector<std::vector<float>>& AudioBuffers::GetInputBuffers() {
    return m_input_buffers;
}

std::vector<std::vector<float>>& AudioBuffers::GetOutputBuffers() {
    return m_output_buffers;
}

void AudioBuffers::EmplaceIntoInput(float* input, unsigned int buffer_size, unsigned int input_channel_count) {

    if (input == nullptr) return;

    for(unsigned int channel_idx = 0; channel_idx < input_channel_count; channel_idx++) {
        for(unsigned int sample_idx = 0; sample_idx < buffer_size; sample_idx++) {
            m_input_buffers[channel_idx][sample_idx] = input[channel_idx * buffer_size + sample_idx];
        }
    }
}

void AudioBuffers::EmplaceIntoOutput(float* output, unsigned int buffer_size, unsigned int output_channel_count) {
    for(unsigned int channel_idx = 0; channel_idx < output_channel_count; channel_idx++) {
        for(unsigned int sample_idx = 0; sample_idx < buffer_size; sample_idx++) {
            m_output_buffers[channel_idx][sample_idx] = output[channel_idx * buffer_size + sample_idx];
        }
    }
}

void AudioBuffers::DumpInputIntoBuffer(float *input, unsigned int buffer_size, unsigned int input_channel_count) {

    if (input == nullptr) return;

    auto* buff = input;
    for(unsigned int sample_idx = 0; sample_idx < buffer_size; sample_idx++) {
        for(unsigned int channel_idx = 0; channel_idx < input_channel_count; channel_idx++) {
            *buff++ = m_input_buffers[channel_idx][sample_idx];
        }
    }
}

void AudioBuffers::DumpOutputIntoBuffer(float *output, unsigned int buffer_size, unsigned int output_channel_count) {
    auto* buff = output;
    for(unsigned int sample_idx = 0; sample_idx < buffer_size; sample_idx++) {
        for(unsigned int channel_idx = 0; channel_idx < output_channel_count; channel_idx++) {
            *buff++ = m_output_buffers[channel_idx][sample_idx];
        }
    }
}
