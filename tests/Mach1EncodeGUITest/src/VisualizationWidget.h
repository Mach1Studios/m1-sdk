#pragma once

#include <vector>
#include <string>
#include <imgui.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Mach1Point3D.h"

class VisualizationWidget {
public:
    VisualizationWidget();
    ~VisualizationWidget();

    void render(const std::vector<std::vector<float>>& gains, 
                const std::vector<Mach1Point3D>& points,
                const std::vector<std::string>& pointNames);
    void renderControls(const std::vector<std::vector<float>>& gains, 
                       const std::vector<Mach1Point3D>& points,
                       const std::vector<std::string>& pointNames);
    void render3DScene(const std::vector<Mach1Point3D>& points,
                      const std::vector<std::string>& pointNames);
    void setupSpatialPoints(int outputChannels);
    void drawPointLabel(const glm::vec4& position, const std::string& label);
    void drawCharacter(std::vector<float>& vertices, char c, float x, float y, float z, float width, float height);
    void addLine(std::vector<float>& vertices, float x1, float y1, float x2, float y2, float z);

private:
    void renderGainMatrix(const std::vector<std::vector<float>>& gains, 
                         const std::vector<std::string>& pointNames);
    void renderGainBars(const std::vector<std::vector<float>>& gains,
                       const std::vector<std::string>& pointNames);
    void renderStatistics(const std::vector<std::vector<float>>& gains);
    void renderChannelAnalysis(const std::vector<std::vector<float>>& gains,
                              const std::vector<std::string>& pointNames);

    // 3D rendering functions
    void setup3DRendering();
    void updateCamera();
    void handleMouseInput();
    void draw3DGrid();
    void draw3DPoint(const Mach1Point3D& point, const std::string& name, bool isSelected = false);
    void draw3DAxis();
    void drawSpatialPoints();
    void drawConnectingLines();
    
    // OpenGL utilities
    void createSphere(float radius, int segments, int rings);
    void createGrid(int size, float spacing);
    void setupShaders();
    void setupRenderTexture();
    void cleanup3DRendering();

    // Helper functions
    ImVec4 getGainColor(float gain, float maxGain = 1.0f);
    void drawGainBar(float gain, float maxGain, const ImVec2& size);

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
    
    // 3D rendering state
    bool m_3DInitialized = false;
    GLuint m_sphereVAO = 0, m_sphereVBO = 0, m_sphereEBO = 0;
    GLuint m_gridVAO = 0, m_gridVBO = 0;
    GLuint m_axisVAO = 0, m_axisVBO = 0;
    GLuint m_shaderProgram = 0;
    
    // Render-to-texture for 3D scene
    GLuint m_framebuffer = 0;
    GLuint m_renderTexture = 0;
    GLuint m_depthBuffer = 0;
    int m_textureWidth = 400;
    int m_textureHeight = 400;
    
    // Camera controls
    float m_cameraDistance = 5.0f;
    float m_cameraRotationX = 0.0f;
    float m_cameraRotationY = 0.0f;
    float m_cameraPanX = 0.0f;
    float m_cameraPanY = 0.0f;
    bool m_mouseDragging = false;
    ImVec2 m_lastMousePos;
    
    // 3D visualization settings
    bool m_showGrid = true;
    bool m_showAxis = true;
    bool m_showSpatialPoints = true;
    bool m_showConnectingLines = true;
    bool m_showPointLabels = false;
    float m_pointSize = 0.1f;
    bool m_autoRotate = false;
    float m_rotationSpeed = 1.0f;
    
    // Spatial point definitions
    std::vector<Mach1Point3D> m_spatialPoints;
    std::vector<std::vector<int>> m_spatialLines;
};
