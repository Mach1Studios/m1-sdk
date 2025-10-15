#include "Mach1EncodeGUI.h"
#include "ParameterControls.h"
#include "VisualizationWidget.h"
#include <iostream>
#include <chrono>

Mach1EncodeGUI::Mach1EncodeGUI() 
    : m_encoder(std::make_unique<Mach1Encode<float>>())
    , m_parameterControls(std::make_unique<ParameterControls>())
    , m_visualizationWidget(std::make_unique<VisualizationWidget>())
{
    // Initialize with default settings
    updateEncodeResults();
}

Mach1EncodeGUI::~Mach1EncodeGUI() = default;

void Mach1EncodeGUI::render() {
    setupDocking();
    renderMenuBar();

    // Main content area
    ImGui::Begin("Mach1Encode Parameter Tester", nullptr, ImGuiWindowFlags_NoCollapse);

    // Split the main window into two panes
    static float splitterWidth = 450.0f; // Increased width for better readability
    static float windowWidth = ImGui::GetContentRegionAvail().x;
    
    // Left pane - Parameter Controls
    ImGui::BeginChild("ParameterControls", ImVec2(splitterWidth, 0), true);
    if (m_parameterControls) {
        m_parameterControls->render(*m_encoder, m_needsUpdate);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Right pane - Visualization
    ImGui::BeginChild("Visualization", ImVec2(0, 0), true);
    if (m_visualizationWidget) {
        m_visualizationWidget->render(m_lastGains, m_lastPoints, m_lastPointNames);
    }
    ImGui::EndChild();

    ImGui::End();

    // Status bar
    renderStatusBar();

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
    ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    
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
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        m_lastCalculationTime = duration.count() / 1000.0f; // Convert to milliseconds
        
    } catch (const std::exception& e) {
        std::cerr << "Error updating encode results: " << e.what() << std::endl;
        m_lastCalculationTime = -1.0f;
    }
}
