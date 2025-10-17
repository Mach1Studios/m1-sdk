#include "VisualizationWidget.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

VisualizationWidget::VisualizationWidget() {
    // Initialize with default settings
}

VisualizationWidget::~VisualizationWidget() {
    cleanup3DRendering();
}

void VisualizationWidget::render(const std::vector<std::vector<float>>& gains, 
                                const std::vector<Mach1Point3D>& points,
                                const std::vector<std::string>& pointNames) {
    // This is the original method - now just calls renderControls
    renderControls(gains, points, pointNames);
}

void VisualizationWidget::renderControls(const std::vector<std::vector<float>>& gains, 
                                        const std::vector<Mach1Point3D>& points,
                                        const std::vector<std::string>& pointNames) {
    ImGui::Text("Visualization Controls");
    ImGui::Separator();

    // 3D Controls
    if (ImGui::CollapsingHeader("3D Controls")) {
        ImGui::SliderFloat("Point Size", &m_pointSize, 0.01f, 0.5f, "%.3f");
        ImGui::Checkbox("Show Grid", &m_showGrid);
        ImGui::Checkbox("Show Axis", &m_showAxis);
        ImGui::Checkbox("Show Spatial Points", &m_showSpatialPoints);
        ImGui::Checkbox("Show Connecting Lines", &m_showConnectingLines);
        ImGui::Checkbox("Show Point Labels", &m_showPointLabels);
        ImGui::Checkbox("Auto Rotate", &m_autoRotate);
        if (m_autoRotate) {
            ImGui::SliderFloat("Rotation Speed", &m_rotationSpeed, 0.1f, 5.0f, "%.1f");
        }
        ImGui::SliderFloat("Camera Distance", &m_cameraDistance, 1.0f, 20.0f, "%.1f");
        
        if (ImGui::Button("Reset Camera")) {
            m_cameraDistance = 5.0f;
            m_cameraRotationX = 0.0f;
            m_cameraRotationY = 0.0f;
            m_cameraPanX = 0.0f;
            m_cameraPanY = 0.0f;
        }
    }

    // Visualization options
    ImGui::Separator();
    ImGui::Text("Display Options");
    ImGui::Checkbox("Gain Matrix", &m_showGainMatrix);
    ImGui::Checkbox("Gain Bars", &m_showGainBars);
    ImGui::Checkbox("Statistics", &m_showStatistics);
    ImGui::Checkbox("Channel Analysis", &m_showChannelAnalysis);

    // Visualization settings
    if (ImGui::CollapsingHeader("Settings")) {
        ImGui::SliderFloat("Gain Scale", &m_gainScale, 0.1f, 5.0f, "%.2f");
        ImGui::Checkbox("Normalize Gains", &m_normalizeGains);
        ImGui::Checkbox("Show Gain Values", &m_showGainValues);
        
        ImGui::ColorEdit3("Positive Gain", (float*)&m_positiveGainColor);
        ImGui::ColorEdit3("Negative Gain", (float*)&m_negativeGainColor);
        ImGui::ColorEdit3("Zero Gain", (float*)&m_zeroGainColor);
    }

    ImGui::Separator();

    // Render non-3D visualization modes
    if (m_showGainMatrix) {
        renderGainMatrix(gains, pointNames);
    }

    if (m_showGainBars) {
        renderGainBars(gains, pointNames);
    }

    if (m_showStatistics) {
        renderStatistics(gains);
    }

    if (m_showChannelAnalysis) {
        renderChannelAnalysis(gains, pointNames);
    }
}

void VisualizationWidget::render3DScene(const std::vector<Mach1Point3D>& points,
                                       const std::vector<std::string>& pointNames) {
    if (points.empty()) return;

    // Setup 3D rendering if not already done
    if (!m_3DInitialized) {
        setup3DRendering();
    }
    
    // Handle mouse input for camera control
    handleMouseInput();
    
    // Update camera for auto-rotation
    if (m_autoRotate) {
        m_cameraRotationY += m_rotationSpeed * 0.01f;
    }
    
    // Get current viewport dimensions
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int viewport_width = viewport[2];
    int viewport_height = viewport[3];
    
    // Setup projection matrix
    float aspect = (float)viewport_width / (float)viewport_height;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    
    // Setup view matrix
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(m_cameraPanX, m_cameraPanY, -m_cameraDistance));
    view = glm::rotate(view, m_cameraRotationX, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, m_cameraRotationY, glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Use shader program
    glUseProgram(m_shaderProgram);
    
    // Set matrices
    GLint projLoc = glGetUniformLocation(m_shaderProgram, "projection");
    GLint viewLoc = glGetUniformLocation(m_shaderProgram, "view");
    GLint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
    
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    // Draw grid
    if (m_showGrid) {
        draw3DGrid();
    }
    
    // Draw axis
    if (m_showAxis) {
        draw3DAxis();
    }
    
    // Draw spatial points and connecting lines
    if (m_showSpatialPoints) {
        drawSpatialPoints();
    }
    
    if (m_showConnectingLines) {
        drawConnectingLines();
    }
    
    // Draw input points (if any)
    for (size_t i = 0; i < points.size(); ++i) {
        const auto& point = points[i];
        bool isSelected = (m_selectedPoint == static_cast<int>(i));
        draw3DPoint(point, pointNames.empty() ? "" : pointNames[i], isSelected);
    }
}

void VisualizationWidget::renderGainMatrix(const std::vector<std::vector<float>>& gains, 
                                          const std::vector<std::string>& pointNames) {
    if (gains.empty()) return;

    ImGui::Text("Gain Matrix");
    
    // Find max gain for normalization
    float maxGain = 0.0f;
    for (const auto& channel : gains) {
        for (float gain : channel) {
            maxGain = std::max(maxGain, std::abs(gain));
        }
    }
    
    if (maxGain == 0.0f) maxGain = 1.0f;

    // Create table
    if (ImGui::BeginTable("GainMatrix", gains[0].size() + 1, 
                         ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY)) {
        
        // Header row
        ImGui::TableSetupColumn("Point", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        for (size_t i = 0; i < gains[0].size(); ++i) {
            ImGui::TableSetupColumn(("Ch" + std::to_string(i)).c_str(), ImGuiTableColumnFlags_WidthFixed, 60.0f);
        }
        ImGui::TableHeadersRow();

        // Data rows
        for (size_t pointIdx = 0; pointIdx < gains.size(); ++pointIdx) {
            ImGui::TableNextRow();
            
            // Point name
            ImGui::TableSetColumnIndex(0);
            std::string pointName = (pointIdx < pointNames.size()) ? pointNames[pointIdx] : ("Point " + std::to_string(pointIdx));
            if (ImGui::Selectable(pointName.c_str(), m_selectedPoint == static_cast<int>(pointIdx))) {
                m_selectedPoint = static_cast<int>(pointIdx);
            }

            // Gain values
            for (size_t chIdx = 0; chIdx < gains[pointIdx].size(); ++chIdx) {
                ImGui::TableSetColumnIndex(static_cast<int>(chIdx + 1));
                
                float gain = gains[pointIdx][chIdx];
                float normalizedGain = m_normalizeGains ? (gain / maxGain) : gain;
                
                ImVec4 color = getGainColor(normalizedGain, 1.0f);
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                
                if (m_showGainValues) {
                    ImGui::Text("%.3f", gain);
                } else {
                    // Draw colored rectangle
                    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
                    ImVec2 size = ImVec2(ImGui::GetColumnWidth(), ImGui::GetTextLineHeight());
                    ImGui::GetWindowDrawList()->AddRectFilled(cursorPos, 
                        ImVec2(cursorPos.x + size.x, cursorPos.y + size.y), 
                        ImGui::ColorConvertFloat4ToU32(color));
                    ImGui::Dummy(size);
                }
                
                ImGui::PopStyleColor();
            }
        }
        
        ImGui::EndTable();
    }
}


void VisualizationWidget::renderGainBars(const std::vector<std::vector<float>>& gains,
                                        const std::vector<std::string>& pointNames) {
    if (gains.empty()) return;

    ImGui::Text("Gain Bars");
    
    // Find max gain for normalization
    float maxGain = 0.0f;
    for (const auto& channel : gains) {
        for (float gain : channel) {
            maxGain = std::max(maxGain, std::abs(gain));
        }
    }
    
    if (maxGain == 0.0f) maxGain = 1.0f;

    // Render gain bars for each point
    for (size_t pointIdx = 0; pointIdx < gains.size(); ++pointIdx) {
        std::string pointName = (pointIdx < pointNames.size()) ? pointNames[pointIdx] : ("Point " + std::to_string(pointIdx));
        
        if (ImGui::CollapsingHeader(pointName.c_str())) {
            ImGui::Indent();
            
            for (size_t chIdx = 0; chIdx < gains[pointIdx].size(); ++chIdx) {
                float gain = gains[pointIdx][chIdx];
                float normalizedGain = m_normalizeGains ? (gain / maxGain) : gain;
                
                ImGui::Text("Ch %zu: %.3f", chIdx, gain);
                drawGainBar(normalizedGain, 1.0f, ImVec2(200, 20));
            }
            
            ImGui::Unindent();
        }
    }
}

void VisualizationWidget::renderStatistics(const std::vector<std::vector<float>>& gains) {
    if (gains.empty()) return;

    ImGui::Text("Statistics");
    
    // Calculate statistics
    float totalGain = 0.0f;
    float maxGain = 0.0f;
    float minGain = 0.0f;
    int nonZeroCount = 0;
    int totalCount = 0;
    
    for (const auto& channel : gains) {
        for (float gain : channel) {
            totalGain += std::abs(gain);
            maxGain = std::max(maxGain, gain);
            minGain = std::min(minGain, gain);
            if (std::abs(gain) > 0.001f) nonZeroCount++;
            totalCount++;
        }
    }
    
    float avgGain = totalCount > 0 ? (totalGain / totalCount) : 0.0f;
    float nonZeroPercentage = totalCount > 0 ? (100.0f * nonZeroCount / totalCount) : 0.0f;
    
    ImGui::Text("Total Points: %zu", gains.size());
    ImGui::Text("Total Channels: %zu", gains.empty() ? 0 : gains[0].size());
    ImGui::Text("Average Gain: %.3f", avgGain);
    ImGui::Text("Max Gain: %.3f", maxGain);
    ImGui::Text("Min Gain: %.3f", minGain);
    ImGui::Text("Non-zero Gains: %d/%d (%.1f%%)", nonZeroCount, totalCount, nonZeroPercentage);
}

void VisualizationWidget::renderChannelAnalysis(const std::vector<std::vector<float>>& gains,
                                               const std::vector<std::string>& pointNames) {
    if (gains.empty()) return;

    ImGui::Text("Channel Analysis");
    
    size_t numChannels = gains[0].size();
    
    for (size_t chIdx = 0; chIdx < numChannels; ++chIdx) {
        if (ImGui::CollapsingHeader(("Channel " + std::to_string(chIdx)).c_str())) {
            ImGui::Indent();
            
            // Calculate channel statistics
            float channelTotal = 0.0f;
            float channelMax = 0.0f;
            int activePoints = 0;
            
            for (size_t pointIdx = 0; pointIdx < gains.size(); ++pointIdx) {
                float gain = gains[pointIdx][chIdx];
                channelTotal += std::abs(gain);
                channelMax = std::max(channelMax, std::abs(gain));
                if (std::abs(gain) > 0.001f) activePoints++;
            }
            
            ImGui::Text("Total Energy: %.3f", channelTotal);
            ImGui::Text("Max Gain: %.3f", channelMax);
            ImGui::Text("Active Points: %d/%zu", activePoints, gains.size());
            
            // Show which points contribute to this channel
            ImGui::Text("Contributing Points:");
            for (size_t pointIdx = 0; pointIdx < gains.size(); ++pointIdx) {
                float gain = gains[pointIdx][chIdx];
                if (std::abs(gain) > 0.001f) {
                    std::string pointName = (pointIdx < pointNames.size()) ? pointNames[pointIdx] : ("Point " + std::to_string(pointIdx));
                    ImGui::BulletText("%s: %.3f", pointName.c_str(), gain);
                }
            }
            
            ImGui::Unindent();
        }
    }
}

ImVec4 VisualizationWidget::getGainColor(float gain, float maxGain) {
    if (std::abs(gain) < 0.001f) {
        return m_zeroGainColor;
    } else if (gain > 0.0f) {
        float intensity = std::min(gain / maxGain, 1.0f);
        return ImVec4(m_positiveGainColor.x * intensity, m_positiveGainColor.y * intensity, 
                     m_positiveGainColor.z * intensity, 1.0f);
    } else {
        float intensity = std::min(-gain / maxGain, 1.0f);
        return ImVec4(m_negativeGainColor.x * intensity, m_negativeGainColor.y * intensity, 
                     m_negativeGainColor.z * intensity, 1.0f);
    }
}

void VisualizationWidget::drawGainBar(float gain, float maxGain, const ImVec2& size) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    
    // Draw background
    drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + size.x, cursorPos.y + size.y), 
                           IM_COL32(50, 50, 50, 255));
    
    // Draw gain bar
    if (std::abs(gain) > 0.001f) {
        float barWidth = (std::abs(gain) / maxGain) * size.x;
        ImVec4 color = getGainColor(gain, maxGain);
        
        if (gain > 0.0f) {
            drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + barWidth, cursorPos.y + size.y), 
                                   ImGui::ColorConvertFloat4ToU32(color));
        } else {
            drawList->AddRectFilled(ImVec2(cursorPos.x + size.x - barWidth, cursorPos.y), 
                                   ImVec2(cursorPos.x + size.x, cursorPos.y + size.y), 
                                   ImGui::ColorConvertFloat4ToU32(color));
        }
    }
    
    // Draw center line
    drawList->AddLine(ImVec2(cursorPos.x + size.x * 0.5f, cursorPos.y), 
                     ImVec2(cursorPos.x + size.x * 0.5f, cursorPos.y + size.y), 
                     IM_COL32(255, 255, 255, 100));
    
    ImGui::Dummy(size);
}

void VisualizationWidget::setup3DRendering() {
    if (m_3DInitialized) return;
    
    // Create simple shader program
    setupShaders();
    
    // Setup render-to-texture
    setupRenderTexture();
    
    // Create sphere geometry
    createSphere(m_pointSize, 16, 16);
    
    // Create grid geometry
    createGrid(20, 0.5f);
    
    // Create axis geometry
    std::vector<float> axisVertices = {
        // X axis (red) - Left to Right
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        // Y axis (green) - Front to Back (Mach1 coordinate system)
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        // Z axis (blue) - Top to Bottom
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f
    };
    
    glGenVertexArrays(1, &m_axisVAO);
    glGenBuffers(1, &m_axisVBO);
    
    glBindVertexArray(m_axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_axisVBO);
    glBufferData(GL_ARRAY_BUFFER, axisVertices.size() * sizeof(float), axisVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    m_3DInitialized = true;
}


void VisualizationWidget::handleMouseInput() {
    ImGuiIO& io = ImGui::GetIO();
    
    // Get current viewport to check if mouse is in 3D area
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int viewport_x = viewport[0];
    int viewport_y = viewport[1];
    int viewport_width = viewport[2];
    int viewport_height = viewport[3];
    
    ImVec2 mousePos = ImGui::GetMousePos();
    bool mouseInViewport = (mousePos.x >= viewport_x && mousePos.x <= viewport_x + viewport_width &&
                           mousePos.y >= viewport_y && mousePos.y <= viewport_y + viewport_height);
    
    if (mouseInViewport && !io.WantCaptureMouse) {
        // Handle mouse wheel for zoom
        if (io.MouseWheel != 0.0f) {
            m_cameraDistance -= io.MouseWheel * 0.5f;
            m_cameraDistance = std::max(1.0f, std::min(20.0f, m_cameraDistance));
        }
        
        // Handle mouse drag for rotation
        if (ImGui::IsMouseDown(0)) {
            if (!m_mouseDragging) {
                m_mouseDragging = true;
                m_lastMousePos = ImGui::GetMousePos();
            } else {
                ImVec2 currentMousePos = ImGui::GetMousePos();
                ImVec2 delta = ImVec2(currentMousePos.x - m_lastMousePos.x, currentMousePos.y - m_lastMousePos.y);
                
                m_cameraRotationY += delta.x * 0.01f;
                m_cameraRotationX += delta.y * 0.01f;
                
                // Clamp rotation
                m_cameraRotationX = std::max(-1.57f, std::min(1.57f, m_cameraRotationX));
                
                m_lastMousePos = currentMousePos;
            }
        } else {
            m_mouseDragging = false;
        }
        
        // Handle right mouse drag for panning
        if (ImGui::IsMouseDown(1)) {
            ImVec2 currentMousePos = ImGui::GetMousePos();
            ImVec2 delta = ImVec2(currentMousePos.x - m_lastMousePos.x, currentMousePos.y - m_lastMousePos.y);
            
            m_cameraPanX += delta.x * 0.01f;
            m_cameraPanY -= delta.y * 0.01f;
            
            m_lastMousePos = currentMousePos;
        }
    } else {
        m_mouseDragging = false;
    }
}

void VisualizationWidget::draw3DGrid() {
    if (m_gridVAO == 0) return;
    
    glm::mat4 model = glm::mat4(1.0f);
    GLint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    glBindVertexArray(m_gridVAO);
    glDrawArrays(GL_LINES, 0, 80); // 20x20 grid = 80 lines
    glBindVertexArray(0);
}

void VisualizationWidget::draw3DAxis() {
    if (m_axisVAO == 0) return;
    
    glm::mat4 model = glm::mat4(1.0f);
    GLint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    glBindVertexArray(m_axisVAO);
    glDrawArrays(GL_LINES, 0, 6); // 3 axes = 6 vertices
    glBindVertexArray(0);
}

void VisualizationWidget::draw3DPoint(const Mach1Point3D& point, const std::string& name, bool isSelected) {
    if (m_sphereVAO == 0) return;
    
    // Create model matrix for this point
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(point.x, point.y, point.z));
    
    // Scale based on selection
    float scale = isSelected ? 1.5f : 1.0f;
    model = glm::scale(model, glm::vec3(scale));
    
    GLint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    // Set color based on selection
    GLint colorLoc = glGetUniformLocation(m_shaderProgram, "color");
    if (isSelected) {
        glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f); // Yellow for selected
    } else {
        glUniform3f(colorLoc, 0.0f, 0.8f, 1.0f); // Blue for normal
    }
    
    glBindVertexArray(m_sphereVAO);
    glDrawElements(GL_TRIANGLES, 1536, GL_UNSIGNED_INT, 0); // 16x16 sphere = 1536 triangles
    glBindVertexArray(0);
}

void VisualizationWidget::createSphere(float radius, int segments, int rings) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // Generate vertices
    for (int i = 0; i <= rings; ++i) {
        float lat = M_PI * (-0.5f + (float)i / rings);
        float y = radius * sin(lat);
        float r = radius * cos(lat);
        
        for (int j = 0; j <= segments; ++j) {
            float lng = 2.0f * M_PI * (float)j / segments;
            float x = r * cos(lng);
            float z = r * sin(lng);
            
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }
    
    // Generate indices
    for (int i = 0; i < rings; ++i) {
        for (int j = 0; j < segments; ++j) {
            int first = i * (segments + 1) + j;
            int second = first + segments + 1;
            
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    
    // Create VAO, VBO, EBO
    glGenVertexArrays(1, &m_sphereVAO);
    glGenBuffers(1, &m_sphereVBO);
    glGenBuffers(1, &m_sphereEBO);
    
    glBindVertexArray(m_sphereVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void VisualizationWidget::createGrid(int size, float spacing) {
    std::vector<float> vertices;
    
    // Generate grid lines
    for (int i = -size/2; i <= size/2; ++i) {
        float pos = i * spacing;
        
        // Vertical lines
        vertices.push_back(pos);
        vertices.push_back(0.0f);
        vertices.push_back(-size/2 * spacing);
        vertices.push_back(pos);
        vertices.push_back(0.0f);
        vertices.push_back(size/2 * spacing);
        
        // Horizontal lines
        vertices.push_back(-size/2 * spacing);
        vertices.push_back(0.0f);
        vertices.push_back(pos);
        vertices.push_back(size/2 * spacing);
        vertices.push_back(0.0f);
        vertices.push_back(pos);
    }
    
    glGenVertexArrays(1, &m_gridVAO);
    glGenBuffers(1, &m_gridVBO);
    
    glBindVertexArray(m_gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gridVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void VisualizationWidget::setupRenderTexture() {
    // Create framebuffer
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    
    // Create texture
    glGenTextures(1, &m_renderTexture);
    glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_textureWidth, m_textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);
    
    // Create depth buffer
    glGenRenderbuffers(1, &m_depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_textureWidth, m_textureHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
    
    // Check framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete!" << std::endl;
    } else {
        std::cout << "Framebuffer setup complete. Texture ID: " << m_renderTexture << std::endl;
    }
    
    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VisualizationWidget::setupShaders() {
    // Simple vertex shader
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        uniform vec3 color;
        
        out vec3 vertexColor;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            vertexColor = color;
        }
    )";
    
    // Simple fragment shader
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 vertexColor;
        out vec4 FragColor;
        
        void main() {
            FragColor = vec4(vertexColor, 1.0);
        }
    )";
    
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // Create shader program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    
    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void VisualizationWidget::cleanup3DRendering() {
    if (m_sphereVAO) {
        glDeleteVertexArrays(1, &m_sphereVAO);
        glDeleteBuffers(1, &m_sphereVBO);
        glDeleteBuffers(1, &m_sphereEBO);
    }
    
    if (m_gridVAO) {
        glDeleteVertexArrays(1, &m_gridVAO);
        glDeleteBuffers(1, &m_gridVBO);
    }
    
    if (m_axisVAO) {
        glDeleteVertexArrays(1, &m_axisVAO);
        glDeleteBuffers(1, &m_axisVBO);
    }
    
    if (m_framebuffer) {
        glDeleteFramebuffers(1, &m_framebuffer);
    }
    
    if (m_renderTexture) {
        glDeleteTextures(1, &m_renderTexture);
    }
    
    if (m_depthBuffer) {
        glDeleteRenderbuffers(1, &m_depthBuffer);
    }
    
    if (m_shaderProgram) {
        glDeleteProgram(m_shaderProgram);
    }
    
    m_3DInitialized = false;
}

void VisualizationWidget::setupSpatialPoints(int outputChannels) {
    m_spatialPoints.clear();
    m_spatialLines.clear();
    
    // Define spatial points based on output channels
    if (outputChannels == 4) {
        // MACH1SPATIAL-4 - Proper plane/square
        m_spatialPoints = {
            {-1, 1, 0},   // 0: Front-left
            {1, 1, 0},    // 1: Front-right
            {-1, -1, 0},  // 2: Back-left
            {1, -1, 0}    // 3: Back-right
        };
        
        // Connecting lines for 4ch - just the quad lines (no Z-axis)
        m_spatialLines = {
            {0, 1}, {1, 3}, {3, 2}, {2, 0}  // Quad lines only
        };
    }
    else if (outputChannels == 8) {
        // MACH1SPATIAL-8
        m_spatialPoints = {
            {-1, 1, 1},   // 0: Top-left-front
            {1, 1, 1},    // 1: Top-right-front
            {-1, -1, 1},  // 2: Top-left-back
            {1, -1, 1},   // 3: Top-right-back
            {-1, 1, -1},  // 4: Bottom-left-Front
            {1, 1, -1},   // 5: Bottom-right-Front
            {-1, -1, -1}, // 6: Bottom-left-back
            {1, -1, -1}   // 7: Bottom-right-back
        };
        
        // Connecting lines for 8ch (cube)
        m_spatialLines = {
            // Top quad lines
            {0, 1}, {1, 3}, {3, 2}, {2, 0},
            // Top to bottom lines
            {0, 4}, {1, 5}, {2, 6}, {3, 7},
            // Bottom quad lines
            {4, 5}, {5, 7}, {7, 6}, {6, 4}
        };
    }
    else if (outputChannels == 14) {
        // MACH1SPATIAL-14
        m_spatialPoints = {
            {-1, 1, 1},   // 0: Top-left-front
            {1, 1, 1},    // 1: Top-right-front
            {-1, -1, 1},  // 2: Bottom-left-front
            {1, -1, 1},   // 3: Bottom-right-front
            {-1, 1, -1},  // 4: Top-left-back
            {1, 1, -1},   // 5: Top-right-back
            {-1, -1, -1}, // 6: Bottom-left-back
            {1, -1, -1},  // 7: Bottom-right-back
            {0, 1.414f, 0},    // 8: Front center
            {1.414f, 0, 0},    // 9: Right center
            {0, -1.414f, 0},   // 10: Back center
            {-1.414f, 0, 0},   // 11: Left center
            {0, 0, 1.414f},    // 12: Top center
            {0, 0, -1.414f}    // 13: Bottom center
        };
        
        // Connecting lines for 14ch
        m_spatialLines = {
            // Cube lines
            {0, 1}, {1, 3}, {3, 2}, {2, 0},
            {0, 4}, {1, 5}, {2, 6}, {3, 7},
            {4, 5}, {5, 7}, {7, 6}, {6, 4},
            // Center point connections
            {0, 8}, {1, 8}, {4, 8}, {5, 8},  // Front
            {1, 9}, {3, 9}, {5, 9}, {7, 9},  // Right
            {3, 10}, {2, 10}, {7, 10}, {6, 10}, // Back
            {0, 11}, {2, 11}, {4, 11}, {6, 11}, // Left
            {0, 12}, {1, 12}, {2, 12}, {3, 12}, // Top
            {4, 13}, {5, 13}, {6, 13}, {7, 13}  // Bottom
        };
    }
    else if (outputChannels == 26) {
        // MACH1SPATIAL-26 - From Mach1EncodeCore.cpp
        m_spatialPoints = {
            // 8ch cube corners
            {-1, 1, 1},   // 0: Top-left-front
            {1, 1, 1},    // 1: Top-right-front
            {-1, -1, 1},  // 2: Bottom-left-front
            {1, -1, 1},   // 3: Bottom-right-front
            {-1, 1, -1},  // 4: Top-left-back
            {1, 1, -1},   // 5: Top-right-back
            {-1, -1, -1}, // 6: Bottom-left-back
            {1, -1, -1},  // 7: Bottom-right-back
            
            // 14ch center points
            {0, 1.414f, 0},    // 8: Top center
            {1.414f, 0, 0},    // 9: Right center
            {0, -1.414f, 0},   // 10: Bottom center
            {-1.414f, 0, 0},   // 11: Left center
            {0, 0, 1.414f},    // 12: Front center
            {0, 0, -1.414f},   // 13: Back center
            
            // 26ch additional edge points
            {0, 1.207, 1.207},     // 14: Top-front edge
            {1.207, 0, 1.207},     // 15: Right-front edge
            {0, -1.207, 1.207},    // 16: Bottom-front edge
            {-1.207, 0, 1.207},    // 17: Left-front edge
            {1.207, 1.207, 0},     // 18: Top-right edge
            {1.207, -1.207, 0},    // 19: Bottom-right edge
            {-1.207, -1.207, 0},   // 20: Bottom-left edge
            {-1.207, 1.207, 0},    // 21: Top-left edge
            {0, 1.207, -1.207},    // 22: Top-back edge
            {1.207, 0, -1.207},    // 23: Right-back edge
            {0, -1.207, -1.207},   // 24: Bottom-back edge
            {-1.207, 0, -1.207}    // 25: Left-back edge
        };
        
        // 26ch connecting lines - using proper edge structure
        m_spatialLines = {
            // Base cube edges (8ch structure)
            // Top face
            {0, 1}, {1, 3}, {3, 2}, {2, 0},
            // Bottom face  
            {4, 5}, {5, 7}, {7, 6}, {6, 4},
            // Vertical edges
            {0, 4}, {1, 5}, {2, 6},
            
            // 14ch center points to cube corners
            // Top center (8) connections
            {8, 0}, {8, 1}, {8, 4}, {8, 5},
            // Right center (9) connections
            {9, 1}, {9, 3}, {9, 5}, {9, 7},
            // Bottom center (10) connections
            {10, 2}, {10, 3}, {10, 6}, {10, 7},
            // Left center (11) connections
            {11, 0}, {11, 2}, {11, 4}, {11, 6},
            // Front center (12) connections
            {12, 0}, {12, 1}, {12, 2}, {12, 3},
            // Back center (13) connections
            {13, 4}, {13, 5}, {13, 6}, {13, 7},
            
            // 26ch edge points - connecting to adjacent corners and centers
            // Front face edge points
            {14, 0}, {14, 1}, {14, 12}, {14, 8},  // Top-front edge
            {15, 1}, {15, 3}, {15, 12}, {15, 9},  // Right-front edge
            {16, 2}, {16, 3}, {16, 12}, {16, 10}, // Bottom-front edge
            {17, 0}, {17, 2}, {17, 12}, {17, 11}, // Left-front edge
            
            // Side face edge points
            {18, 5}, {18, 8}, {18, 1}, {18, 9},   // Top-right edge
            {19, 3}, {19, 7}, {19, 9}, {19, 10},  // Bottom-right edge
            {20, 2}, {20, 6}, {20, 10}, {20, 11}, // Bottom-left edge
            {21, 0}, {21, 4}, {21, 8}, {21, 11},  // Top-left edge
            
            // Back face edge points
            {22, 4}, {22, 5}, {22, 13}, {22, 8},  // Top-back edge
            {23, 5}, {23, 7}, {23, 13}, {23, 9},  // Right-back edge
            {24, 6}, {24, 7}, {24, 13}, {24, 10}, // Bottom-back edge
            {25, 4}, {25, 6}, {25, 13}, {25, 11}  // Left-back edge
        };
    }
    else if (outputChannels == 38) {
        // MACH1SPATIAL-38 - Complete implementation from Mach1EncodeCore.cpp
        m_spatialPoints = {
            // 8ch cube
            {-1, 1, 1}, {1, 1, 1}, {-1, -1, 1}, {1, -1, 1},
            {-1, 1, -1}, {1, 1, -1}, {-1, -1, -1}, {1, -1, -1},
            // 14ch additions
            {0, 1.414f, 0}, {1.414f, 0, 0}, {0, -1.414f, 0}, {-1.414f, 0, 0},
            {0, 0, 1.414f}, {0, 0, -1.414f},
            // 38ch additional points
            {0.0, 1.473370419, -0.910592997},
            {0.618033989, 1.618033989, 0.0},
            {0.0, 1.473370419, 0.910592997},
            {-0.618033989, 1.618033989, 0.0},
            {1.473370419, 0.910592997, 0.0},
            {-1.473370419, 0.910592997, 0.0},
            {0.0, 0.618033989, -1.618033989},
            {0.0, 0.618033989, 1.618033989},
            {0.910592997, 0.0, -1.473370419},
            {1.618033989, 0.0, -0.618033989},
            {1.618033989, 0.0, 0.618033989},
            {0.910592997, 0.0, 1.473370419},
            {-0.910592997, 0.0, 1.473370419},
            {-1.618033989, 0.0, 0.618033989},
            {-1.618033989, 0.0, -0.618033989},
            {-0.910592997, 0.0, -1.473370419},
            {0.0, -0.618033989, -1.618033989},
            {0.0, -0.618033989, 1.618033989},
            {1.473370419, -0.910592997, 0.0},
            {-1.473370419, -0.910592997, 0.0},
            {0.0, -1.473370419, -0.910592997},
            {0.618033989, -1.618033989, 0.0},
            {0.0, -1.473370419, 0.910592997},
            {-0.618033989, -1.618033989, 0.0}
        };
        
        // 38ch connecting lines - using proper edge structure to avoid intersecting lines
        // This creates a wireframe of the 38ch shape without internal intersecting lines
        m_spatialLines = {
            // Base cube edges (8ch structure)
            // Top face
            {0, 1}, {1, 3}, {3, 2}, {2, 0},
            // Bottom face  
            {4, 5}, {5, 7}, {7, 6}, {6, 4},
            // Vertical edges
            {0, 4}, {1, 5}, {2, 6}, {3, 7},
            
            // 14ch additions - center points to cube corners
            // Front center (8) connections
            {8, 0}, {8, 1}, {8, 4}, {8, 5},
            // Right center (9) connections
            {9, 1}, {9, 3}, {9, 5}, {9, 7},
            // Back center (10) connections
            {10, 2}, {10, 3}, {10, 6}, {10, 7},
            // Left center (11) connections
            {11, 0}, {11, 2}, {11, 4}, {11, 6},
            // Top center (12) connections
            {12, 0}, {12, 1}, {12, 2}, {12, 3},
            // Bottom center (13) connections
            {13, 4}, {13, 5}, {13, 6}, {13, 7},
            
            // 38ch additional points - key structural edges only
            // Front face edges
            {14, 15}, {15, 16}, {16, 17}, {17, 14},
            // Back face edges
            {18, 19}, {19, 20}, {20, 21}, {21, 18},
            // Left face edges
            {22, 23}, {23, 24}, {24, 25}, {25, 22},
            // Right face edges
            {26, 27}, {27, 28}, {28, 29}, {29, 26},
            // Top face edges
            {30, 31}, {31, 32}, {32, 33}, {33, 30},
            // Bottom face edges
            {34, 35}, {35, 36}, {36, 37}, {37, 34},
            
            // Key connecting edges between faces
            {14, 18}, {15, 19}, {16, 20}, {17, 21},
            {22, 26}, {23, 27}, {24, 28}, {25, 29},
            {30, 34}, {31, 35}, {32, 36}, {33, 37}
        };
    }
}

void VisualizationWidget::drawSpatialPoints() {
    if (m_spatialPoints.empty()) return;
    
    // Set color for spatial points (green)
    GLint colorLoc = glGetUniformLocation(m_shaderProgram, "color");
    glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);
    
    // Transform from Mach1 coordinate system to OpenGL coordinate system
    // Mach1: X=left-right, Y=front-back, Z=top-bottom
    // OpenGL: X=left-right, Y=up-down, Z=back-front
    // For 4-channel points (Z=0 in Mach1), we want them on the floor (Y=0 in OpenGL)
    glm::mat4 coordTransform = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    for (size_t i = 0; i < m_spatialPoints.size(); ++i) {
        const auto& point = m_spatialPoints[i];
        
        // Transform the point from Mach1 coordinates to OpenGL coordinates
        glm::vec4 transformedPoint = coordTransform * glm::vec4(point.x, point.y, point.z, 1.0f);
        
        // Create model matrix for this point
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(transformedPoint.x, transformedPoint.y, transformedPoint.z));
        model = glm::scale(model, glm::vec3(m_pointSize * 2.0f)); // Make spatial points larger
        
        GLint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        glBindVertexArray(m_sphereVAO);
        glDrawElements(GL_TRIANGLES, 1536, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Draw point label if enabled
        if (m_showPointLabels) {
            drawPointLabel(transformedPoint, std::to_string(i));
        }
    }
}

void VisualizationWidget::drawConnectingLines() {
    if (m_spatialLines.empty() || m_spatialPoints.empty()) return;
    
    // Set color for connecting lines (white)
    GLint colorLoc = glGetUniformLocation(m_shaderProgram, "color");
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
    
    // Transform from Mach1 coordinate system to OpenGL coordinate system
    glm::mat4 coordTransform = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    // Create line geometry
    std::vector<float> lineVertices;
    for (const auto& line : m_spatialLines) {
        if (line.size() >= 2) {
            const auto& p1 = m_spatialPoints[line[0]];
            const auto& p2 = m_spatialPoints[line[1]];
            
            // Transform both points from Mach1 coordinates to OpenGL coordinates
            glm::vec4 transformedP1 = coordTransform * glm::vec4(p1.x, p1.y, p1.z, 1.0f);
            glm::vec4 transformedP2 = coordTransform * glm::vec4(p2.x, p2.y, p2.z, 1.0f);
            
            lineVertices.insert(lineVertices.end(), {transformedP1.x, transformedP1.y, transformedP1.z});
            lineVertices.insert(lineVertices.end(), {transformedP2.x, transformedP2.y, transformedP2.z});
        }
    }
    
    if (lineVertices.empty()) return;
    
    // Create temporary VAO for lines
    GLuint lineVAO, lineVBO;
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    
    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(float), lineVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Draw lines
    glm::mat4 model = glm::mat4(1.0f);
    GLint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, lineVertices.size() / 3);
    glBindVertexArray(0);
    
    // Cleanup
    glDeleteVertexArrays(1, &lineVAO);
    glDeleteBuffers(1, &lineVBO);
}

void VisualizationWidget::drawPointLabel(const glm::vec4& position, const std::string& label) {
    if (label.empty()) return;
    
    // Set color for labels (bright white)
    GLint colorLoc = glGetUniformLocation(m_shaderProgram, "color");
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
    
    // Create proper text rendering using geometric shapes for each character
    std::vector<float> labelVertices;
    float charWidth = 0.15f;
    float charHeight = 0.2f;
    float labelHeight = 0.8f;
    
    // Position the label above the point
    float labelX = position.x;
    float labelY = position.y;
    float labelZ = position.z + labelHeight;
    
    // Calculate total width for centering
    float totalWidth = label.length() * charWidth * 0.8f; // 0.8f for spacing
    float startX = labelX - totalWidth / 2.0f;
    
    // Render each character
    for (size_t i = 0; i < label.length() && i < 5; ++i) { // Limit to 5 characters max
        char c = label[i];
        float charX = startX + i * charWidth * 0.8f;
        
        // Draw character using line segments
        drawCharacter(labelVertices, c, charX, labelY, labelZ, charWidth, charHeight);
    }
    
    if (labelVertices.empty()) return;
    
    // Create temporary VAO and VBO for label
    GLuint labelVAO, labelVBO;
    glGenVertexArrays(1, &labelVAO);
    glGenBuffers(1, &labelVBO);
    
    glBindVertexArray(labelVAO);
    glBindBuffer(GL_ARRAY_BUFFER, labelVBO);
    glBufferData(GL_ARRAY_BUFFER, labelVertices.size() * sizeof(float), labelVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Set model matrix to identity
    glm::mat4 model = glm::mat4(1.0f);
    GLint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    // Draw label lines
    glBindVertexArray(labelVAO);
    glDrawArrays(GL_LINES, 0, labelVertices.size() / 3);
    glBindVertexArray(0);
    
    // Cleanup
    glDeleteVertexArrays(1, &labelVAO);
    glDeleteBuffers(1, &labelVBO);
}

void VisualizationWidget::drawCharacter(std::vector<float>& vertices, char c, float x, float y, float z, float width, float height) {
    // Simple bitmap font rendering using line segments
    // Each character is defined by line segments
    
    switch (c) {
        case '0':
            // Draw '0' as a rectangle
            addLine(vertices, x, y, x + width, y, z);                    // top
            addLine(vertices, x + width, y, x + width, y + height, z);   // right
            addLine(vertices, x + width, y + height, x, y + height, z);  // bottom
            addLine(vertices, x, y + height, x, y, z);                   // left
            break;
        case '1':
            // Draw '1' as a vertical line with a small top line
            addLine(vertices, x + width/2, y, x + width/2, y + height, z); // vertical
            break;
        case '2':
            // Draw '2' as segments
            addLine(vertices, x, y, x + width, y, z);                    // top
            addLine(vertices, x + width, y, x + width, y + height/2, z); // right top
            addLine(vertices, x + width, y + height/2, x, y + height/2, z); // middle
            addLine(vertices, x, y + height/2, x, y + height, z);        // left bottom
            addLine(vertices, x, y + height, x + width, y + height, z);  // bottom
            break;
        case '3':
            // Draw '3' as segments
            addLine(vertices, x, y, x + width, y, z);                    // top
            addLine(vertices, x + width, y, x + width, y + height, z);   // right
            addLine(vertices, x, y + height/2, x + width, y + height/2, z); // middle
            addLine(vertices, x, y + height, x + width, y + height, z);  // bottom
            break;
        case '4':
            // Draw '4' as segments
            addLine(vertices, x, y, x, y + height/2, z);                 // left top
            addLine(vertices, x, y + height/2, x + width, y + height/2, z); // middle
            addLine(vertices, x + width, y, x + width, y + height, z);   // right
            break;
        case '5':
            // Draw '5' as segments
            addLine(vertices, x + width, y, x, y, z);                    // top
            addLine(vertices, x, y, x, y + height/2, z);                 // left top
            addLine(vertices, x, y + height/2, x + width, y + height/2, z); // middle
            addLine(vertices, x + width, y + height/2, x + width, y + height, z); // right bottom
            addLine(vertices, x + width, y + height, x, y + height, z);  // bottom
            break;
        case '6':
            // Draw '6' as segments
            addLine(vertices, x + width, y, x, y, z);                    // top
            addLine(vertices, x, y, x, y + height, z);                   // left
            addLine(vertices, x, y + height/2, x + width, y + height/2, z); // middle
            addLine(vertices, x + width, y + height/2, x + width, y + height, z); // right bottom
            addLine(vertices, x + width, y + height, x, y + height, z);  // bottom
            break;
        case '7':
            // Draw '7' as segments
            addLine(vertices, x, y, x + width, y, z);                    // top
            addLine(vertices, x + width, y, x + width, y + height, z);   // right
            break;
        case '8':
            // Draw '8' as two rectangles
            addLine(vertices, x, y, x + width, y, z);                    // top
            addLine(vertices, x + width, y, x + width, y + height/2, z); // right top
            addLine(vertices, x + width, y + height/2, x, y + height/2, z); // middle
            addLine(vertices, x, y + height/2, x, y, z);                 // left top
            addLine(vertices, x, y + height/2, x, y + height, z);        // left bottom
            addLine(vertices, x, y + height, x + width, y + height, z);  // bottom
            addLine(vertices, x + width, y + height, x + width, y + height/2, z); // right bottom
            break;
        case '9':
            // Draw '9' as segments
            addLine(vertices, x, y, x + width, y, z);                    // top
            addLine(vertices, x + width, y, x + width, y + height, z);   // right
            addLine(vertices, x, y, x, y + height/2, z);                 // left top
            addLine(vertices, x, y + height/2, x + width, y + height/2, z); // middle
            addLine(vertices, x + width, y + height, x, y + height, z);  // bottom
            break;
        default:
            // For unknown characters, draw a simple cross
            addLine(vertices, x, y, x + width, y + height, z);
            addLine(vertices, x + width, y, x, y + height, z);
            break;
    }
}

void VisualizationWidget::addLine(std::vector<float>& vertices, float x1, float y1, float x2, float y2, float z) {
    vertices.insert(vertices.end(), {x1, y1, z, x2, y2, z});
}
