#ifndef MACH1SPATIALSDK_AUDIOBUFFERS_H
#define MACH1SPATIALSDK_AUDIOBUFFERS_H

#include <vector>

namespace Mach1 {

class AudioBuffers {
public:
    ~AudioBuffers() = default;

    AudioBuffers() = default;
    AudioBuffers(const AudioBuffers& other) = default;
    AudioBuffers& operator=(const AudioBuffers& other) = default;
    AudioBuffers(AudioBuffers&& other) = default;
    AudioBuffers& operator=(AudioBuffers&& other) = default;

    AudioBuffers(unsigned int buffer_size, unsigned int input_channel_count, unsigned int output_channel_count);

    void Restructure(unsigned int buffer_size, unsigned int input_channel_count, unsigned int output_channel_count);

    unsigned int GetBufferSize() const;
    unsigned int GetInputChannelCount() const;
    unsigned int GetOutputChannelCount() const;

    std::vector<std::vector<float>>& GetInputBuffers();
    std::vector<std::vector<float>>& GetOutputBuffers();

    void EmplaceIntoInput(float* input, unsigned int buffer_size, unsigned int input_channel_count);
    void EmplaceIntoOutput(float* output, unsigned int buffer_size, unsigned int output_channel_count);

    void DumpInputIntoBuffer(float* input, unsigned int buffer_size, unsigned int input_channel_count);
    void DumpOutputIntoBuffer(float* output, unsigned int buffer_size, unsigned int output_channel_count);

private:
    std::vector<std::vector<float>> m_output_buffers;
    std::vector<std::vector<float>> m_input_buffers;

    unsigned int m_buffer_size{};
    unsigned int m_input_channel_count{};
    unsigned int m_output_channel_count{};
};

} // Mach1

#endif //MACH1SPATIALSDK_AUDIOBUFFERS_H
