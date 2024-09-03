#include "DecodeLink.h"

using namespace Mach1;

void DecodeLink::Process(AudioBuffers &buffers, double playback_time) {
    m_decode.decodeBuffer(
            &buffers.GetOutputBuffers(),
            &buffers.GetOutputBuffers(),
            m_points,
            buffers.GetBufferSize()
    );
}

void DecodeLink::SetPlatformType(Mach1PlatformType type) {
    m_decode.setPlatformType(type);
}

void DecodeLink::SetDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType) {
    m_decode.setDecodeAlgoType(newAlgorithmType);
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
