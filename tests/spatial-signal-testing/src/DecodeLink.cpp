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

    // process the samples manually
    for (int decode_coeffs = 0; decode_coeffs < decode_gains.size(); decode_coeffs+=2) {
        for (int sample = 0; sample < buffers.GetBufferSize(); sample++) {

            // get the sample in each loop
            float val = output_buffer[decode_coeffs/2][sample];

            // clear the output for the new values to come in
            output_buffer[decode_coeffs/2][sample] = 0;

            output_buffer[0][sample] += val * decode_gains[decode_coeffs + 0];
            output_buffer[1][sample] += val * decode_gains[decode_coeffs + 1];
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
