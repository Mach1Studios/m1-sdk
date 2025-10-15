#pragma once

#include <imgui.h>
#include "Mach1Encode.h"

class ParameterControls {
public:
    ParameterControls();
    ~ParameterControls() = default;

    void render(Mach1Encode<float>& encoder, bool& needsUpdate);

private:
    void renderInputOutputModes(Mach1Encode<float>& encoder, bool& needsUpdate);
    void renderSpatialParameters(Mach1Encode<float>& encoder, bool& needsUpdate);
    void renderAdvancedParameters(Mach1Encode<float>& encoder, bool& needsUpdate);
    void renderPresets(Mach1Encode<float>& encoder, bool& needsUpdate);

    // Helper functions
    const char* getInputModeName(Mach1EncodeInputMode mode);
    const char* getOutputModeName(Mach1EncodeOutputMode mode);
    const char* getPannerModeName(Mach1EncodePannerMode mode);
    
    // State variables for UI
    int m_selectedInputMode = 0;
    int m_selectedOutputMode = 0;
    int m_selectedPannerMode = 0;
    
    // Parameter values
    float m_azimuthDegrees = 0.0f;
    float m_elevationDegrees = 0.0f;
    float m_diverge = 0.0f;
    float m_stereoSpread = 0.0f;
    float m_orbitRotationDegrees = 0.0f;
    float m_outputGain = 6.0f; // Default +6dB
    bool m_autoGain = true;
    bool m_autoOrbit = true;
    bool m_frontSurroundPerspective = true;
    
    // Input type selection
    int m_azimuthInputType = 1; // 0: normalized, 1: degrees, 2: radians
    int m_elevationInputType = 1; // 0: normalized, 1: degrees, 2: radians
    int m_orbitInputType = 1; // 0: normalized, 1: degrees, 2: radians
};
