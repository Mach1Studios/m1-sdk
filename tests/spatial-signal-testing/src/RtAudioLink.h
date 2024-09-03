#ifndef MACH1SPATIALSDK_RTAUDIOLINK_H
#define MACH1SPATIALSDK_RTAUDIOLINK_H


#include "AudioBuffers.h"

namespace Mach1 {

class RtAudioLink {
public:
    virtual ~RtAudioLink() = default;

    RtAudioLink() = default;
    RtAudioLink(const RtAudioLink& other) = delete;
    RtAudioLink& operator=(const RtAudioLink& other) = delete;
    RtAudioLink(RtAudioLink&& other) = delete;
    RtAudioLink& operator=(RtAudioLink&& other) = delete;

    virtual void Process(AudioBuffers& buffers, double playback_time) = 0;

    virtual void SetSampleRate(size_t sample_rate) {};

    virtual void SetOutputChannelCount(unsigned int output_channel_count) {};
};

} // Mach1

#endif //MACH1SPATIALSDK_RTAUDIOLINK_H
