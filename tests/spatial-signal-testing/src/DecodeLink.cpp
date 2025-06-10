#include "DecodeLink.h"
#include "Settings.h"


using namespace Mach1;

void DecodeLink::Process(AudioBuffers &buffers, double playback_time) {
#if REBUFFERED_DECODE
    m_decode.decodeBufferInPlaceRebuffer(buffers.GetOutputBuffers(), buffers.GetBufferSize());
#else
    m_decode.decodeBufferInPlace(buffers.GetOutputBuffers(), buffers.GetBufferSize());
#endif
}

void DecodeLink::SetPlatformType(Mach1PlatformType type) {
    m_decode.setPlatformType(type);
}

void DecodeLink::SetDecodeMode(Mach1DecodeMode mode) {
    m_decode.setDecodeMode(mode);
}

void DecodeLink::SetRotation(Mach1Point3D newRotationFromMinusOnetoOne) {
    m_decode.setRotation(newRotationFromMinusOnetoOne);
}

void DecodeLink::SetRotationDegrees(Mach1Point3D newRotationDegrees) {
    m_decode.setRotationDegrees(newRotationDegrees);
}

void DecodeLink::SetRotationRadians(Mach1Point3D newRotationRadians) {
    m_decode.setRotationRadians(newRotationRadians);
}

void DecodeLink::SetRotationQuat(Mach1Point4D newRotationQuat) {
    m_decode.setRotationQuat(newRotationQuat);
}

void DecodeLink::SetFilterSpeed(float filterSpeed) {
    m_decode.setFilterSpeed(filterSpeed);
}

void DecodeLink::SetPointCount(int points) {
    m_points = points;
}
