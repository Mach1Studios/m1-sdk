#ifndef MACH1SPATIALSDK_DECODELINK_H
#define MACH1SPATIALSDK_DECODELINK_H

#include "RtAudioLink.h"

namespace Mach1 {

class DecodeLink : public virtual RtAudioLink {
public:
    void Process(AudioBuffers &buffers, double playback_time) override;

};

} // Mach1

#endif //MACH1SPATIALSDK_DECODELINK_H
