#pragma once

#include <vector>
#include <string>
#include <imgui.h>
#include "Mach1Point3D.h"

class VisualizationWidget {
public:
    VisualizationWidget();
    ~VisualizationWidget() = default;

    void render(const std::vector<std::vector<float>>& gains, 
                const std::vector<Mach1Point3D>& points,
                const std::vector<std::string>& pointNames);

private:
    void renderGainMatrix(const std::vector<std::vector<float>>& gains, 
                         const std::vector<std::string>& pointNames);
    void render3DVisualization(const std::vector<Mach1Point3D>& points,
                              const std::vector<std::string>& pointNames);
    void renderGainBars(const std::vector<std::vector<float>>& gains,
                       const std::vector<std::string>& pointNames);
    void renderStatistics(const std::vector<std::vector<float>>& gains);
    void renderChannelAnalysis(const std::vector<std::vector<float>>& gains,
                              const std::vector<std::string>& pointNames);

    // Helper functions
    ImVec4 getGainColor(float gain, float maxGain = 1.0f);
    void drawGainBar(float gain, float maxGain, const ImVec2& size);
    void draw3DPoint(const Mach1Point3D& point, const std::string& name, bool isSelected = false);

    // State
    int m_selectedPoint = -1;
    int m_selectedChannel = -1;
    bool m_showGainMatrix = true;
    bool m_show3DVisualization = true;
    bool m_showGainBars = true;
    bool m_showStatistics = true;
    bool m_showChannelAnalysis = false;
    
    // Visualization settings
    float m_gainScale = 1.0f;
    bool m_normalizeGains = false;
    bool m_showGainValues = true;
    ImVec4 m_positiveGainColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 m_negativeGainColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 m_zeroGainColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
};
