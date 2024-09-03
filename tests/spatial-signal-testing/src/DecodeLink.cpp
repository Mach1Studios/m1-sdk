#include "DecodeLink.h"

using namespace Mach1;

void DecodeLink::Process(AudioBuffers &buffers, double playback_time) {
    // TODO: fix decodeBuffer
    /*
    m_decode.decodeBuffer(
            &buffers.GetOutputBuffers(),
            &buffers.GetOutputBuffers(),
            m_points,
            buffers.GetBufferSize()
    );
     */

    // get output gain multipliers
    auto decode_gains = m_decode.decodeCoeffs();

    // process the samples manually
    for (int sample = 0; sample < buffers.m_buffer_size; sample++) {
        for (int input_channel = 0; input_channel < buffers.m_input_channel_count; input_channel++) {

            //std::vector<float> outBufferL = buffers.GetOutputBuffers()[0]
            //std::vector<float> outBufferR = buffers.GetOutputBuffers()[1]

            //outBufferL[sample] += tempBuffer.getReadPointer(input_channel * 2    )[sample] * smoothedChannelCoeffs[input_channel_reordered][0].getNextValue();
            //outBufferR[sample] += tempBuffer.getReadPointer(input_channel * 2 + 1)[sample] * smoothedChannelCoeffs[input_channel_reordered][1].getNextValue();
        }
    }
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

std::vector<float> DecodeLink::GetGains() {
    return m_decode.decodeCoeffs();
}
