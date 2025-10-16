#pragma once

#include <memory>
#include <vector>
#include <string>
#include <imgui.h>
#include "Mach1Encode.h"

class ParameterControls;
class VisualizationWidget;

class Mach1EncodeGUI {
public:
    Mach1EncodeGUI();
    ~Mach1EncodeGUI();

    void render();
    void render3DScene();

private:
    void setupDocking();
    void renderMenuBar();
    void renderStatusBar();
    void updateEncodeResults();

    // GUI Components
    std::unique_ptr<ParameterControls> m_parameterControls;
    std::unique_ptr<VisualizationWidget> m_visualizationWidget;

    // Mach1Encode instance
    std::unique_ptr<Mach1Encode<float>> m_encoder;

    // State
    bool m_showDemo = false;
    bool m_showAbout = false;
    bool m_autoUpdate = true;
    bool m_needsUpdate = true;
    
    // Results cache
    std::vector<std::vector<float>> m_lastGains;
    std::vector<Mach1Point3D> m_lastPoints;
    std::vector<std::string> m_lastPointNames;
    
    // Statistics
    float m_lastCalculationTime = 0.0f;
    int m_inputChannels = 0;
    int m_outputChannels = 0;
    int m_pointsCount = 0;
};
