#include "VisualizationWidget.h"
#include <algorithm>
#include <numeric>
#include <cmath>

VisualizationWidget::VisualizationWidget() {
    // Initialize with default settings
}

void VisualizationWidget::render(const std::vector<std::vector<float>>& gains, 
                                const std::vector<Mach1Point3D>& points,
                                const std::vector<std::string>& pointNames) {
    ImGui::Text("Visualization");
    ImGui::Separator();

    // Visualization options
    ImGui::Checkbox("Gain Matrix", &m_showGainMatrix);
    ImGui::SameLine();
    ImGui::Checkbox("3D Points", &m_show3DVisualization);
    ImGui::SameLine();
    ImGui::Checkbox("Gain Bars", &m_showGainBars);
    
    ImGui::Checkbox("Statistics", &m_showStatistics);
    ImGui::SameLine();
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

    // Render different visualization modes
    if (m_showGainMatrix) {
        renderGainMatrix(gains, pointNames);
    }

    if (m_show3DVisualization) {
        render3DVisualization(points, pointNames);
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

void VisualizationWidget::render3DVisualization(const std::vector<Mach1Point3D>& points,
                                               const std::vector<std::string>& pointNames) {
    if (points.empty()) return;

    ImGui::Text("3D Spatial Points");
    
    // Create a simple 2D projection of the 3D points
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImVec2(300, 300);
    
    // Draw canvas background
    ImGui::InvisibleButton("3DCanvas", canvasSize);
    drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), 
                           IM_COL32(50, 50, 50, 255));
    
    // Draw grid
    for (int i = 0; i <= 10; ++i) {
        float t = i / 10.0f;
        // Vertical lines
        drawList->AddLine(
            ImVec2(canvasPos.x + t * canvasSize.x, canvasPos.y),
            ImVec2(canvasPos.x + t * canvasSize.x, canvasPos.y + canvasSize.y),
            IM_COL32(100, 100, 100, 100)
        );
        // Horizontal lines
        drawList->AddLine(
            ImVec2(canvasPos.x, canvasPos.y + t * canvasSize.y),
            ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + t * canvasSize.y),
            IM_COL32(100, 100, 100, 100)
        );
    }
    
    // Draw points
    for (size_t i = 0; i < points.size(); ++i) {
        const auto& point = points[i];
        bool isSelected = (m_selectedPoint == static_cast<int>(i));
        
        // Convert 3D coordinates to 2D screen coordinates
        // X: left-right (Z coordinate), Y: front-back (X coordinate)
        float screenX = canvasPos.x + (point.z + 1.0f) * 0.5f * canvasSize.x;
        float screenY = canvasPos.y + (1.0f - point.x) * 0.5f * canvasSize.y;
        
        // Draw point
        ImU32 pointColor = isSelected ? IM_COL32(255, 255, 0, 255) : IM_COL32(255, 255, 255, 255);
        float pointRadius = isSelected ? 8.0f : 5.0f;
        drawList->AddCircleFilled(ImVec2(screenX, screenY), pointRadius, pointColor);
        
        // Draw point label
        if (i < pointNames.size()) {
            drawList->AddText(ImVec2(screenX + 10, screenY - 10), IM_COL32(255, 255, 255, 255), 
                             pointNames[i].c_str());
        }
        
        // Handle point selection
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            float mouseX = ImGui::GetMousePos().x - canvasPos.x;
            float mouseY = ImGui::GetMousePos().y - canvasPos.y;
            
            float dist = sqrtf((mouseX - screenX) * (mouseX - screenX) + (mouseY - screenY) * (mouseY - screenY));
            if (dist < pointRadius + 5.0f) {
                m_selectedPoint = static_cast<int>(i);
            }
        }
    }
    
    // Draw coordinate labels
    drawList->AddText(ImVec2(canvasPos.x + 10, canvasPos.y + 10), IM_COL32(255, 255, 255, 255), "Front");
    drawList->AddText(ImVec2(canvasPos.x + canvasSize.x - 30, canvasPos.y + 10), IM_COL32(255, 255, 255, 255), "Back");
    drawList->AddText(ImVec2(canvasPos.x + 10, canvasPos.y + canvasSize.y - 20), IM_COL32(255, 255, 255, 255), "Left");
    drawList->AddText(ImVec2(canvasPos.x + canvasSize.x - 30, canvasPos.y + canvasSize.y - 20), IM_COL32(255, 255, 255, 255), "Right");
    
    ImGui::Dummy(canvasSize);
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

void VisualizationWidget::draw3DPoint(const Mach1Point3D& point, const std::string& name, bool isSelected) {
    // This is a placeholder for more advanced 3D rendering
    // In a full implementation, you might use OpenGL or a 3D library
    ImGui::Text("Point: %s (%.3f, %.3f, %.3f)", name.c_str(), point.x, point.y, point.z);
}
