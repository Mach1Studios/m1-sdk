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
    auto &output_buffer = buffers.GetOutputBuffers();

    for (int sample = 0; sample < buffers.GetBufferSize(); sample++) {
        for (int output_channel = 0; output_channel < buffers.GetOutputChannelCount(); output_channel++) {
            output_buffer[output_channel][sample] = 0;
        }
    }

    // process the samples manually
    for (int sample = 0; sample < buffers.GetBufferSize(); sample++) {
        for (int input_channel = 0; input_channel < buffers.GetInputChannelCount(); input_channel++) {
            output_buffer[0][sample] += output_buffer[input_channel * 2    ][sample] * decode_gains[input_channel];
            output_buffer[1][sample] += output_buffer[input_channel * 2 + 1][sample] * decode_gains[input_channel];
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
