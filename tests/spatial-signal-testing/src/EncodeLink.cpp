#include "EncodeLink.h"

using namespace Mach1;

void EncodeLink::Process(AudioBuffers &buffers, double playback_time) {
    // TODO: fix decodeBuffer
    /*
    m_encode.encodeBuffer<float>(
            &buffers.GetOutputBuffers(),
            &buffers.GetOutputBuffers(),
            buffers.GetBufferSize()
    );
     */

    // get output gain multipliers
    auto encode_gains = m_encode.getGains();

    // process the samples manually
    for (int sample = 0; sample < buffers.m_buffer_size; sample++) {
        for (int input_channel = 0; input_channel < buffers.m_input_channel_count; input_channel++) {
            for (int output_channel = 0; output_channel < buffers.m_output_channel_count; output_channel++){
                //buffers.GetOutputBuffers()[input_channel][output_channel].setTargetValue(encode_gains[input_channel][output_channel]);
            }
        }
    }
}

void EncodeLink::SetInputMode(Mach1EncodeInputModeType inputMode) {
    m_encode.setInputMode(inputMode);
}

void EncodeLink::SetOutputMode(Mach1EncodeOutputModeType outputMode) {
    m_encode.setOutputMode(outputMode);
}

void EncodeLink::SetAzimuth(float azimuthFromMinus1To1) {
    m_encode.setAzimuth(azimuthFromMinus1To1);
}

void EncodeLink::SetAzimuthDegrees(float azimuthDegrees) {
    m_encode.setAzimuthDegrees(azimuthDegrees);
}

void EncodeLink::SetAzimuthRadians(float azimuthRadians) {
    m_encode.setAzimuthRadians(azimuthRadians);
}

void EncodeLink::SetElevation(float elevationFromMinus1to1) {
    m_encode.setElevation(elevationFromMinus1to1);
}

void EncodeLink::SetElevationDegrees(float elevationFromMinus90to90) {
    m_encode.setElevationDegrees(elevationFromMinus90to90);
}

void EncodeLink::SetElevationRadians(float elevationFromMinusHalfPItoHalfPI) {
    m_encode.setElevationRadians(elevationFromMinusHalfPItoHalfPI);
}

void EncodeLink::SetFrontSurroundPerspective(bool frontSurroundPerspective) {
    m_encode.setFrontSurroundPerspective(frontSurroundPerspective);
}

void EncodeLink::SetOutputGain(float outputGainMultipler, bool isDecibel) {
    m_encode.setOutputGain(outputGainMultipler, isDecibel);
}

void EncodeLink::SetOrbitRotation(float orbitRotationFromMinusOnetoOne) {
    m_encode.setOrbitRotation(orbitRotationFromMinusOnetoOne);
}

void EncodeLink::SetOrbitRotationDegrees(float orbitRotationDegrees) {
    m_encode.setOrbitRotationDegrees(orbitRotationDegrees);
}

void EncodeLink::SetOrbitRotationRadians(float orbitRotationRadians) {
    m_encode.setOrbitRotationRadians(orbitRotationRadians);
}

void EncodeLink::SetDiverge(float divergeFromMinus1To1) {
    m_encode.setDiverge(divergeFromMinus1To1);
}

void EncodeLink::SetStereoSpread(float sSpreadFrom0to1) {
    m_encode.setStereoSpread(sSpreadFrom0to1);
}

void EncodeLink::SetAutoOrbit(bool autoOrbit) {
    m_encode.setAutoOrbit(autoOrbit);
}

void EncodeLink::GeneratePointResults() {
    m_encode.generatePointResults();
}

std::vector< std::vector<float> > EncodeLink::GetGains() {
    return m_encode.getGains();
}

int EncodeLink::GetPointsCount() {
    return m_encode.getPointsCount();
}

void EncodeLink::SetEqualPowerMode(bool is_on) {
    m_is_equal_power = is_on;
    ResolvePannerMode();
}

void EncodeLink::SetIsotropicMode(bool is_on) {
    m_is_isotropic = is_on;
    ResolvePannerMode();
}

void EncodeLink::ResolvePannerMode() {

    if (m_is_isotropic) {
        if (m_is_equal_power) {
            m_encode.setPannerMode(Mach1EncodePannerModeType::Mach1EncodePannerModeIsotropicEqualPower);
        } else {
            m_encode.setPannerMode(Mach1EncodePannerModeType::Mach1EncodePannerModeIsotropicLinear);
        }
    } else {
        m_encode.setPannerMode(Mach1EncodePannerModeType::Mach1EncodePannerModePeriphonicLinear);
    }

}
