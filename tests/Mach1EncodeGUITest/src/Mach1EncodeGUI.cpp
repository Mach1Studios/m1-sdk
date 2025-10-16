#include "Mach1EncodeGUI.h"
#include "ParameterControls.h"
#include "VisualizationWidget.h"
#include <iostream>
#include <chrono>
#include <GL/glew.h>

Mach1EncodeGUI::Mach1EncodeGUI() 
    : m_encoder(std::make_unique<Mach1Encode<float>>())
    , m_parameterControls(std::make_unique<ParameterControls>())
    , m_visualizationWidget(std::make_unique<VisualizationWidget>())
{
    // Set default output mode to 4-channel
    m_encoder->setOutputMode(M1Spatial_4);
    
    // Initialize with default settings
    updateEncodeResults();
}

Mach1EncodeGUI::~Mach1EncodeGUI() = default;

void Mach1EncodeGUI::render() {
    // Render as a side panel (no window decorations)
    ImGui::Begin("Mach1Encode Controls", nullptr, 
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Parameter Controls
    if (m_parameterControls) {
        m_parameterControls->render(*m_encoder, m_needsUpdate);
    }

    ImGui::Separator();

    // Visualization Controls (non-3D parts)
    if (m_visualizationWidget) {
        m_visualizationWidget->renderControls(m_lastGains, m_lastPoints, m_lastPointNames);
    }

    ImGui::End();

    // Status bar
    renderStatusBar();
}

void Mach1EncodeGUI::render3DScene() {
    // Clear the 3D viewport
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);
    
    // Render the 3D scene using the visualization widget
    if (m_visualizationWidget) {
        m_visualizationWidget->render3DScene(m_lastPoints, m_lastPointNames);
    }
    
    // Disable depth testing
    glDisable(GL_DEPTH_TEST);

    // About dialog
    if (m_showAbout) {
        ImGui::Begin("About Mach1Encode GUI Test Tool", &m_showAbout, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Mach1Encode GUI Test Tool v1.0");
        ImGui::Text("Interactive testing and visualization tool for Mach1Encode API");
        ImGui::Separator();
        ImGui::Text("Features:");
        ImGui::BulletText("Real-time parameter adjustment");
        ImGui::BulletText("Gain coefficient visualization");
        ImGui::BulletText("3D spatial point visualization");
        ImGui::BulletText("Multiple input/output format support");
        ImGui::Separator();
        ImGui::Text("Built with Dear ImGui and OpenGL");
        if (ImGui::Button("Close")) {
            m_showAbout = false;
        }
        ImGui::End();
    }

    // ImGui demo window
    if (m_showDemo) {
        ImGui::ShowDemoWindow(&m_showDemo);
    }

    // Update results if needed
    if (m_needsUpdate && m_autoUpdate) {
        updateEncodeResults();
        m_needsUpdate = false;
    }
}

void Mach1EncodeGUI::setupDocking() {
    // Simplified version without docking for compatibility
    // This can be enhanced later when docking is available
}

void Mach1EncodeGUI::renderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Reset Parameters", "Ctrl+R")) {
                // Reset to defaults
                m_encoder = std::make_unique<Mach1Encode<float>>();
                m_needsUpdate = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                // Will be handled by main loop
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Auto Update", nullptr, &m_autoUpdate);
            ImGui::MenuItem("Show Demo Window", nullptr, &m_showDemo);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
                m_showAbout = true;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void Mach1EncodeGUI::renderStatusBar() {
    // Position the status bar at the bottom right of the window
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 window_pos = ImVec2(viewport->Pos.x + viewport->Size.x - 700, viewport->Pos.y + viewport->Size.y - 30);
    ImVec2 window_size = ImVec2(700, 30);
    
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);
    
    ImGui::Begin("Status", nullptr, 
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoBackground);
    
    ImGui::Text("Input Channels: %d | Output Channels: %d | Points: %d", 
                m_inputChannels, m_outputChannels, m_pointsCount);
    ImGui::SameLine();
    ImGui::Text("| Calculation Time: %.3f ms", m_lastCalculationTime);
    
    if (m_needsUpdate) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "| Update Pending");
    }
    
    ImGui::End();
}

void Mach1EncodeGUI::updateEncodeResults() {
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        // Generate new results
        m_encoder->generatePointResults();
        
        // Get the results
        m_lastGains = m_encoder->getGains();
        m_lastPoints = m_encoder->getPoints();
        m_lastPointNames = m_encoder->getPointsNames();
        
        // Update statistics
        m_inputChannels = m_encoder->getInputChannelsCount();
        m_outputChannels = m_encoder->getOutputChannelsCount();
        m_pointsCount = m_encoder->getPointsCount();
        
        // Setup spatial points for the current output mode
        if (m_visualizationWidget) {
            m_visualizationWidget->setupSpatialPoints(m_outputChannels);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        m_lastCalculationTime = duration.count() / 1000.0f; // Convert to milliseconds
        
    } catch (const std::exception& e) {
        std::cerr << "Error updating encode results: " << e.what() << std::endl;
        m_lastCalculationTime = -1.0f;
    }
}
