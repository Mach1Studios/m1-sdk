#include "ParameterControls.h"
#include <iostream>

ParameterControls::ParameterControls() {
    // Initialize with default values
}

void ParameterControls::render(Mach1Encode<float>& encoder, bool& needsUpdate) {
    ImGui::Text("Parameter Controls");
    ImGui::Separator();

    // Make all sections open by default with better spacing
    if (ImGui::CollapsingHeader("Input/Output Modes", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent();
        renderInputOutputModes(encoder, needsUpdate);
        ImGui::Unindent();
        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("Spatial Parameters", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent();
        renderSpatialParameters(encoder, needsUpdate);
        ImGui::Unindent();
        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("Advanced Parameters", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent();
        renderAdvancedParameters(encoder, needsUpdate);
        ImGui::Unindent();
        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("Presets", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent();
        renderPresets(encoder, needsUpdate);
        ImGui::Unindent();
        ImGui::Spacing();
    }

    // Manual update button
    ImGui::Separator();
    if (ImGui::Button("Update Results", ImVec2(-1, 0))) {
        needsUpdate = true;
    }
    ImGui::Text("(or enable auto-update)");
}

void ParameterControls::renderInputOutputModes(Mach1Encode<float>& encoder, bool& needsUpdate) {
    // Input Mode Selection
    const char* inputModes[] = {
        "Mono", "Stereo", "LCR", "Quad", "LCRS", "AFormat",
        "5.0", "5.1 Film", "5.1 DTS", "5.1 SMPTE",
        "1OA ACN", "1OA FUMA", "2OA ACN", "2OA FUMA", "3OA ACN", "3OA FUMA"
    };
    
    if (ImGui::Combo("Input Mode", &m_selectedInputMode, inputModes, IM_ARRAYSIZE(inputModes))) {
        encoder.setInputMode(static_cast<Mach1EncodeInputMode>(m_selectedInputMode));
        needsUpdate = true;
    }

    // Output Mode Selection
    const char* outputModes[] = {
        "M1Spatial-4", "M1Spatial-8", "M1Spatial-14", "M1Spatial-38"
    };
    
    if (ImGui::Combo("Output Mode", &m_selectedOutputMode, outputModes, IM_ARRAYSIZE(outputModes))) {
        encoder.setOutputMode(static_cast<Mach1EncodeOutputMode>(m_selectedOutputMode));
        needsUpdate = true;
    }

    // Panner Mode Selection
    const char* pannerModes[] = {
        "Isotropic Linear", "Isotropic Equal Power", "Periphonic Linear"
    };
    
    if (ImGui::Combo("Panner Mode", &m_selectedPannerMode, pannerModes, IM_ARRAYSIZE(pannerModes))) {
        encoder.setPannerMode(static_cast<Mach1EncodePannerMode>(m_selectedPannerMode));
        needsUpdate = true;
    }

    // Display current channel counts
    ImGui::Text("Input Channels: %d", encoder.getInputChannelsCount());
    ImGui::Text("Output Channels: %d", encoder.getOutputChannelsCount());
}

void ParameterControls::renderSpatialParameters(Mach1Encode<float>& encoder, bool& needsUpdate) {
    // Azimuth Control
    ImGui::Text("Azimuth");
    const char* azimuthTypes[] = { "Normalized (-1 to 1)", "Degrees (0 to 360)", "Radians (-π to π)" };
    if (ImGui::Combo("Azimuth Type", &m_azimuthInputType, azimuthTypes, IM_ARRAYSIZE(azimuthTypes))) {
        needsUpdate = true;
    }

    float azimuthMin = -1.0f, azimuthMax = 1.0f;
    const char* azimuthFormat = "%.3f";
    
    switch (m_azimuthInputType) {
        case 0: // Normalized
            azimuthMin = -1.0f; azimuthMax = 1.0f;
            azimuthFormat = "%.3f";
            break;
        case 1: // Degrees
            azimuthMin = 0.0f; azimuthMax = 360.0f;
            azimuthFormat = "%.1f°";
            break;
        case 2: // Radians
            azimuthMin = -3.14159f; azimuthMax = 3.14159f;
            azimuthFormat = "%.3f rad";
            break;
    }

    if (ImGui::SliderFloat("Azimuth", &m_azimuthDegrees, azimuthMin, azimuthMax, azimuthFormat)) {
        switch (m_azimuthInputType) {
            case 0: encoder.setAzimuth(m_azimuthDegrees); break;
            case 1: encoder.setAzimuthDegrees(m_azimuthDegrees); break;
            case 2: encoder.setAzimuthRadians(m_azimuthDegrees); break;
        }
        needsUpdate = true;
    }

    // Elevation Control
    ImGui::Text("Elevation");
    const char* elevationTypes[] = { "Normalized (-1 to 1)", "Degrees (-90 to 90)", "Radians (-π/2 to π/2)" };
    if (ImGui::Combo("Elevation Type", &m_elevationInputType, elevationTypes, IM_ARRAYSIZE(elevationTypes))) {
        needsUpdate = true;
    }

    float elevationMin = -1.0f, elevationMax = 1.0f;
    const char* elevationFormat = "%.3f";
    
    switch (m_elevationInputType) {
        case 0: // Normalized
            elevationMin = -1.0f; elevationMax = 1.0f;
            elevationFormat = "%.3f";
            break;
        case 1: // Degrees
            elevationMin = -90.0f; elevationMax = 90.0f;
            elevationFormat = "%.1f°";
            break;
        case 2: // Radians
            elevationMin = -1.5708f; elevationMax = 1.5708f;
            elevationFormat = "%.3f rad";
            break;
    }

    if (ImGui::SliderFloat("Elevation", &m_elevationDegrees, elevationMin, elevationMax, elevationFormat)) {
        switch (m_elevationInputType) {
            case 0: encoder.setElevation(m_elevationDegrees); break;
            case 1: encoder.setElevationDegrees(m_elevationDegrees); break;
            case 2: encoder.setElevationRadians(m_elevationDegrees); break;
        }
        needsUpdate = true;
    }

    // Diverge Control
    if (ImGui::SliderFloat("Diverge", &m_diverge, -1.0f, 1.0f, "%.3f")) {
        encoder.setDiverge(m_diverge);
        needsUpdate = true;
    }

    // Stereo Spread Control
    if (ImGui::SliderFloat("Stereo Spread", &m_stereoSpread, 0.0f, 1.0f, "%.3f")) {
        encoder.setStereoSpread(m_stereoSpread);
        needsUpdate = true;
    }

    // Orbit Rotation Control
    ImGui::Text("Orbit Rotation");
    const char* orbitTypes[] = { "Normalized (-1 to 1)", "Degrees (-180 to 180)", "Radians (-π to π)" };
    if (ImGui::Combo("Orbit Type", &m_orbitInputType, orbitTypes, IM_ARRAYSIZE(orbitTypes))) {
        needsUpdate = true;
    }

    float orbitMin = -1.0f, orbitMax = 1.0f;
    const char* orbitFormat = "%.3f";
    
    switch (m_orbitInputType) {
        case 0: // Normalized
            orbitMin = -1.0f; orbitMax = 1.0f;
            orbitFormat = "%.3f";
            break;
        case 1: // Degrees
            orbitMin = -180.0f; orbitMax = 180.0f;
            orbitFormat = "%.1f°";
            break;
        case 2: // Radians
            orbitMin = -3.14159f; orbitMax = 3.14159f;
            orbitFormat = "%.3f rad";
            break;
    }

    if (ImGui::SliderFloat("Orbit Rotation", &m_orbitRotationDegrees, orbitMin, orbitMax, orbitFormat)) {
        switch (m_orbitInputType) {
            case 0: encoder.setOrbitRotation(m_orbitRotationDegrees); break;
            case 1: encoder.setOrbitRotationDegrees(m_orbitRotationDegrees); break;
            case 2: encoder.setOrbitRotationRadians(m_orbitRotationDegrees); break;
        }
        needsUpdate = true;
    }
}

void ParameterControls::renderAdvancedParameters(Mach1Encode<float>& encoder, bool& needsUpdate) {
    // Auto Gain Compensation
    if (ImGui::Checkbox("Auto Gain Compensation", &m_autoGain)) {
        encoder.setGainCompensationActive(m_autoGain);
        needsUpdate = true;
    }

    // Auto Orbit
    if (ImGui::Checkbox("Auto Orbit", &m_autoOrbit)) {
        encoder.setAutoOrbit(m_autoOrbit);
        needsUpdate = true;
    }

    // Front Surround Perspective
    if (ImGui::Checkbox("Front Surround Perspective", &m_frontSurroundPerspective)) {
        encoder.setFrontSurroundPerspective(m_frontSurroundPerspective);
        needsUpdate = true;
    }

    // Output Gain
    if (ImGui::SliderFloat("Output Gain (dB)", &m_outputGain, -20.0f, 20.0f, "%.1f dB")) {
        encoder.setOutputGain(m_outputGain, true); // true = decibel mode
        needsUpdate = true;
    }

    // Display current gain compensation value
    float gainComp = encoder.getGainCompensation(true);
    ImGui::Text("Gain Compensation: %.2f dB", gainComp);
}

void ParameterControls::renderPresets(Mach1Encode<float>& encoder, bool& needsUpdate) {
    if (ImGui::Button("Center (0°, 0°)")) {
        m_azimuthDegrees = 0.0f;
        m_elevationDegrees = 0.0f;
        m_diverge = 0.0f;
        encoder.setAzimuthDegrees(m_azimuthDegrees);
        encoder.setElevationDegrees(m_elevationDegrees);
        encoder.setDiverge(m_diverge);
        needsUpdate = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Front (0°, 0°)")) {
        m_azimuthDegrees = 0.0f;
        m_elevationDegrees = 0.0f;
        m_diverge = 1.0f;
        encoder.setAzimuthDegrees(m_azimuthDegrees);
        encoder.setElevationDegrees(m_elevationDegrees);
        encoder.setDiverge(m_diverge);
        needsUpdate = true;
    }

    if (ImGui::Button("Right (90°, 0°)")) {
        m_azimuthDegrees = 90.0f;
        m_elevationDegrees = 0.0f;
        m_diverge = 1.0f;
        encoder.setAzimuthDegrees(m_azimuthDegrees);
        encoder.setElevationDegrees(m_elevationDegrees);
        encoder.setDiverge(m_diverge);
        needsUpdate = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Back (180°, 0°)")) {
        m_azimuthDegrees = 180.0f;
        m_elevationDegrees = 0.0f;
        m_diverge = 1.0f;
        encoder.setAzimuthDegrees(m_azimuthDegrees);
        encoder.setElevationDegrees(m_elevationDegrees);
        encoder.setDiverge(m_diverge);
        needsUpdate = true;
    }

    if (ImGui::Button("Left (270°, 0°)")) {
        m_azimuthDegrees = 270.0f;
        m_elevationDegrees = 0.0f;
        m_diverge = 1.0f;
        encoder.setAzimuthDegrees(m_azimuthDegrees);
        encoder.setElevationDegrees(m_elevationDegrees);
        encoder.setDiverge(m_diverge);
        needsUpdate = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Top (0°, 90°)")) {
        m_azimuthDegrees = 0.0f;
        m_elevationDegrees = 90.0f;
        m_diverge = 1.0f;
        encoder.setAzimuthDegrees(m_azimuthDegrees);
        encoder.setElevationDegrees(m_elevationDegrees);
        encoder.setDiverge(m_diverge);
        needsUpdate = true;
    }

    if (ImGui::Button("Bottom (0°, -90°)")) {
        m_azimuthDegrees = 0.0f;
        m_elevationDegrees = -90.0f;
        m_diverge = 1.0f;
        encoder.setAzimuthDegrees(m_azimuthDegrees);
        encoder.setElevationDegrees(m_elevationDegrees);
        encoder.setDiverge(m_diverge);
        needsUpdate = true;
    }
}

const char* ParameterControls::getInputModeName(Mach1EncodeInputMode mode) {
    switch (mode) {
        case Mono: return "Mono";
        case Stereo: return "Stereo";
        case LCR: return "LCR";
        case Quad: return "Quad";
        case LCRS: return "LCRS";
        case AFormat: return "AFormat";
        case FiveDotZero: return "5.0";
        case FiveDotOneFilm: return "5.1 Film";
        case FiveDotOneDTS: return "5.1 DTS";
        case FiveDotOneSMTPE: return "5.1 SMPTE";
        case BFOAACN: return "1OA ACN";
        case BFOAFUMA: return "1OA FUMA";
        case B2OAACN: return "2OA ACN";
        case B2OAFUMA: return "2OA FUMA";
        case B3OAACN: return "3OA ACN";
        case B3OAFUMA: return "3OA FUMA";
        default: return "Unknown";
    }
}

const char* ParameterControls::getOutputModeName(Mach1EncodeOutputMode mode) {
    switch (mode) {
        case M1Spatial_4: return "M1Spatial-4";
        case M1Spatial_8: return "M1Spatial-8";
        case M1Spatial_14: return "M1Spatial-14";
        case M1Spatial_38: return "M1Spatial-38";
        default: return "Unknown";
    }
}

const char* ParameterControls::getPannerModeName(Mach1EncodePannerMode mode) {
    switch (mode) {
        case IsotropicLinear: return "Isotropic Linear";
        case IsotropicEqualPower: return "Isotropic Equal Power";
        case PeriphonicLinear: return "Periphonic Linear";
        default: return "Unknown";
    }
}
