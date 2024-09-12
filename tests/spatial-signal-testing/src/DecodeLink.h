#ifndef MACH1SPATIALSDK_DECODELINK_H
#define MACH1SPATIALSDK_DECODELINK_H

#include "RtAudioLink.h"

#include <Mach1Decode.h>

namespace Mach1 {

class DecodeLink : public virtual RtAudioLink {
public:
    void Process(AudioBuffers &buffers, double playback_time) override;

    void SetPlatformType(Mach1PlatformType type);
    void SetDecodeMode(Mach1DecodeMode mode);

    void SetRotation(Mach1Point3D newRotationFromMinusOnetoOne);
    void SetRotationDegrees(Mach1Point3D newRotationDegrees);
    void SetRotationRadians(Mach1Point3D newRotationRadians);
    void SetRotationQuat(Mach1Point4D newRotationQuat);

    void SetFilterSpeed(float filterSpeed);

    void SetPointCount(int points);

private:
    Mach1Decode m_decode{};

    int m_points{};
};

} // Mach1

#endif //MACH1SPATIALSDK_DECODELINK_H
