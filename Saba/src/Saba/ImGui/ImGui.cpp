#include <pch.h>

#define IMGUI_IMPLEMENTATION
#include <misc/single_file/imgui_single_file.h>

#if defined(SB_PLATFORM_WINDOWS)
#   include <backends/imgui_impl_win32.cpp>
#   include <backends/imgui_impl_dx11.cpp>
//#   include <backends/imgui_impl_dx12.cpp>
#else
#   include <backends/imgui_impl_glfw.cpp>
#endif

#include <backends/imgui_impl_opengl3.cpp>
//#include <backends/imgui_impl_vulkan.cpp>

#include "ImGui.h"

#include "Saba/Renderer/RendererAPI.h"

namespace Saba::UI {

void DrawImage(Ref<Texture2D> texture, ImVec2 size) {
    if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
        ImGui::Image((uint64_t)texture->GetRawPointer(), size, { 0, 1 }, { 1, 0 });
    else
        ImGui::Image((uint64_t)texture->GetRawPointer(), size);
}
void DrawImage(Ref<RenderTarget> renderTarget, ImVec2 size) {
    if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
        ImGui::Image((uint64_t)renderTarget->GetRawTexturePointer(), size, { 0, 1 }, { 1, 0 });
    else
        ImGui::Image((uint64_t)renderTarget->GetRawTexturePointer(), size);
}

bool DragFloat(const std::string& label, float& value, float resetValue, float minVal, float maxVal, float speed, float columnWidth)
{
    bool changed = false;
    auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::TextUnformatted(label.c_str());
    ImGui::NextColumn();

    ImGui::PushItemWidth(-0.1f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        value = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    changed |= ImGui::DragFloat("###X", &value, speed, minVal, maxVal);
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
    return changed;
}
bool DragFloat2(const std::string& label, glm::vec2& values, float resetValue, float minVal, float maxVal, float speed, float columnWidth) {
    bool changed = false;
    auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::TextUnformatted(label.c_str());
    ImGui::NextColumn();

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        values.x = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    changed |= ImGui::DragFloat("###X", &values.x, speed, minVal, maxVal);
    ImGui::PopItemWidth();


    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 2.0f - lineHeight - 6.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        values.y = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 2.0f - lineHeight - 6.0f);
    changed |= ImGui::DragFloat("###Y", &values.y, speed, minVal, maxVal);
    ImGui::PopItemWidth();


    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
    return changed;
}
bool DragFloat3(const std::string& label, glm::vec3& values, float resetValue, float minVal, float maxVal, float speed, float columnWidth) {
    bool changed = false;
    auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::TextUnformatted(label.c_str());
    ImGui::NextColumn();

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        values.x = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.0f - lineHeight - 6.0f);
    changed |= ImGui::DragFloat("###X", &values.x, speed, minVal, maxVal);
    ImGui::PopItemWidth();


    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        values.y = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.0f - lineHeight - 6.0f);
    changed |= ImGui::DragFloat("###Y", &values.y, speed, minVal, maxVal);
    ImGui::PopItemWidth();


    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) {
        values.z = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.0f - lineHeight - 6.0f);
    changed |= ImGui::DragFloat("###Z", &values.z, speed, minVal, maxVal);
    ImGui::PopItemWidth();


    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
    return changed;
}
bool DragFloat4(const std::string& label, glm::vec4& values, float resetValue, float minVal, float maxVal, float speed, float columnWidth) {
    bool changed = false;
    auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::TextUnformatted(label.c_str());
    ImGui::NextColumn();

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        values.x = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 4.0f - lineHeight - 6.0f);
    changed |= ImGui::DragFloat("###X", &values.x, speed, minVal, maxVal);
    ImGui::PopItemWidth();


    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        values.y = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 4.0f - lineHeight - 6.0f);
    changed |= ImGui::DragFloat("###Y", &values.y, speed, minVal, maxVal);
    ImGui::PopItemWidth();


    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) {
        values.z = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 4.0f - lineHeight - 6.0f);
    changed |= ImGui::DragFloat("###Z", &values.z, speed, minVal, maxVal);
    ImGui::PopItemWidth();


    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.8f, 0.1f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.9f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.8f, 0.1f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("W", buttonSize)) {
        values.w = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 4.0f - lineHeight - 6.0f);
    changed |= ImGui::DragFloat("###W", &values.w, speed, minVal, maxVal);
    ImGui::PopItemWidth();


    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
    return changed;
}

bool DragInt(const std::string& label, int& value, int resetValue, int minVal, int maxVal, float speed, float columnWidth) {
    bool changed = false;
    auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::TextUnformatted(label.c_str());
    ImGui::NextColumn();

    ImGui::PushItemWidth(-0.1f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        value = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    changed |= ImGui::DragInt("###X", &value, speed, minVal, maxVal);
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
    return changed;
}
bool DragInt2(const std::string& label, glm::ivec2& values, int resetValue, int minVal, int maxVal, float speed, float columnWidth) {
    bool changed = false;
    auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::TextUnformatted(label.c_str());
    ImGui::NextColumn();

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        values.x = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 2.0f - lineHeight - 6.0f);
    changed |= ImGui::DragInt("###X", &values.x, speed, minVal, maxVal);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        values.y = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 2.0f - lineHeight - 6.0f);
    changed |= ImGui::DragInt("###Y", &values.y, speed, minVal, maxVal);
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
    return changed;
}
bool DragInt3(const std::string& label, glm::ivec3& values, int resetValue, int minVal, int maxVal, float speed, float columnWidth) {
    bool changed = false;
    auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::TextUnformatted(label.c_str());
    ImGui::NextColumn();

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        values.x = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.0f - lineHeight - 6.0f);
    changed |= ImGui::DragInt("###X", &values.x, speed, minVal, maxVal);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        values.y = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.0f - lineHeight - 6.0f);
    changed |= ImGui::DragInt("###Y", &values.y, speed, minVal, maxVal);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) {
        values.z = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.0f - lineHeight - 6.0f);
    changed |= ImGui::DragInt("###Z", &values.z, speed, minVal, maxVal);
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
    return changed;
}
bool DragInt4(const std::string& label, glm::ivec4& values, int resetValue, int minVal, int maxVal, float speed, float columnWidth) {
    bool changed = false;
    auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::TextUnformatted(label.c_str());
    ImGui::NextColumn();

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        values.x = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 4.0f - lineHeight - 6.0f);
    changed |= ImGui::DragInt("###X", &values.x, speed, minVal, maxVal);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        values.y = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 4.0f - lineHeight - 6.0f);
    changed |= ImGui::DragInt("###Y", &values.y, speed, minVal, maxVal);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) {
        values.z = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 4.0f - lineHeight - 6.0f);
    changed |= ImGui::DragInt("###Z", &values.z, speed, minVal, maxVal);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.8f, 0.1f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.9f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.8f, 0.1f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("W", buttonSize)) {
        values.w = resetValue;
        changed = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 4.0f - lineHeight - 6.0f);
    changed |= ImGui::DragInt("###W", &values.w, speed, minVal, maxVal);
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
    return changed;
}

bool ColorEdit3(const std::string& label, glm::vec3& values, float columnWidth) {
    ImGui::PushID(label.c_str());
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::TextUnformatted(label.c_str());
    ImGui::NextColumn();

    ImGui::PushItemWidth(-0.1f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
    bool changed = ImGui::ColorEdit3("###C", &values.x);
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    return changed;
}
bool ColorEdit4(const std::string& label, glm::vec4& values, float columnWidth) {
    ImGui::PushID(label.c_str());
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::TextUnformatted(label.c_str());
    ImGui::NextColumn();

    ImGui::PushItemWidth(-0.1f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
    bool changed = ImGui::ColorEdit4("###C", &values.x);
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    return changed;
}

void HelpMarker(const char* desc) { //From imgui_demo.cpp
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void ShiftCursorX(float x) noexcept {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x);
}
void ShiftCursorY(float y) noexcept {
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + y);
}
void ShiftCursor(float x, float y) noexcept {
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + x, ImGui::GetCursorPosY() + y));
}

ImRect GetItemRect() noexcept {
    return { ImGui::GetItemRectMin(), ImGui::GetItemRectMax() };
}
ImRect RectExpand(const ImRect &rect, float x, float y) noexcept {
    ImRect result;
    result.Min = { rect.Min.x - x, rect.Min.y - y };
    result.Max = { rect.Max.x + x, rect.Max.y + y };
    return result;
}
ImRect RectMove(const ImRect& rect, float x, float y) noexcept {
    ImRect result;
    result.Min = { rect.Min.x + x, rect.Min.y + y };
    result.Max = { rect.Max.x + x, rect.Max.y + y };
    return result;
}

bool BeginMenuBar(const ImRect& menuBarRect) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    SB_CORE_ASSERT(!window->DC.MenuBarAppending);
    ImGui::BeginGroup();
    ImGui::PushID("##menubar");

    const auto padding = window->WindowPadding;
    const auto barRect = RectMove(menuBarRect, 0.0f, padding.y);
    ImRect clipRect(IM_ROUND(ImMax(window->Pos.x, barRect.Min.x + window->WindowBorderSize + window->Pos.x - 10.0f)),
        IM_ROUND(barRect.Min.y + window->WindowBorderSize + window->Pos.y),
        IM_ROUND(ImMax(barRect.Min.x + window->Pos.x, barRect.Max.x - ImMax(window->WindowRounding, window->WindowBorderSize))),
        IM_ROUND(barRect.Max.y + window->Pos.y));

    //debug
    // ImGui::GetForegroundDrawList()->AddRect(clipRect.Min, clipRect.Max, IM_COL32(0xffu, 0x20u, 0x20u, 0xffu));

    clipRect.ClipWith(window->OuterRectClipped);
    ImGui::PushClipRect(clipRect.Min, clipRect.Max, false);

    window->DC.CursorPos = window->DC.CursorMaxPos = { barRect.Min.x + window->Pos.x, barRect.Min.y + window->Pos.y };
    window->DC.LayoutType = ImGuiLayoutType_Horizontal;
    window->DC.NavLayerCurrent = ImGuiNavLayer_Menu;
    window->DC.MenuBarAppending = true;
    ImGui::AlignTextToFramePadding();
    return true;
}
void EndMenuBar() {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;
    ImGuiContext& g = *GImGui;

    if (ImGui::NavMoveRequestButNoResultYet() && (g.NavMoveDir == ImGuiDir_Left || g.NavMoveDir == ImGuiDir_Right) && ((g.NavWindow->Flags & ImGuiWindowFlags_ChildMenu) != 0)) {
        ImGuiWindow* navEarliestChild = g.NavWindow;
        while (navEarliestChild->ParentWindow != nullptr && ((navEarliestChild->Flags & ImGuiWindowFlags_ChildMenu) != 0))
            navEarliestChild = navEarliestChild->ParentWindow;

        if (navEarliestChild->ParentWindow == window && navEarliestChild->DC.ParentLayoutType == ImGuiLayoutType_Horizontal &&
            (g.NavMoveFlags & ImGuiNavMoveFlags_Forwarded) == 0)
        {
            const ImGuiNavLayer layer = ImGuiNavLayer_Menu;
            SB_CORE_ASSERT(window->DC.NavLayersActiveMaskNext & (1 << layer));
            ImGui::FocusWindow(window);
            ImGui::SetNavID(window->NavLastIds[layer], layer, 0, window->NavRectRel[layer]);
            g.NavCursorVisible = false;
            g.NavHighlightItemUnderNav = g.NavMousePosDirty = true;
            ImGui::NavMoveRequestForward(g.NavMoveDir, g.NavMoveClipDir, g.NavMoveFlags, g.NavMoveScrollFlags);
        }
    }

    SB_CORE_ASSERT(window->DC.MenuBarAppending);
    ImGui::PopClipRect();
    ImGui::PopID();
    window->DC.MenuBarOffset.x = window->DC.CursorPos.x - window->Pos.x;
    g.GroupStack.back().EmitItem = false;
    ImGui::EndGroup();
    window->DC.LayoutType = ImGuiLayoutType_Vertical;
    window->DC.NavLayerCurrent = ImGuiNavLayer_Main;
    window->DC.MenuBarAppending = false;
}

void DrawButtonImage(Ref<Texture2D>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImVec2 rectMin, ImVec2 rectMax) noexcept {
    auto* drawList = ImGui::GetForegroundDrawList();
    if (ImGui::IsItemActive())
        drawList->AddImage((uint64_t)(image->GetRawPointer()), rectMin, rectMax, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintPressed);
    else if (ImGui::IsItemHovered())
        drawList->AddImage((uint64_t)(image->GetRawPointer()), rectMin, rectMax, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintHovered);
    else
        drawList->AddImage((uint64_t)(image->GetRawPointer()), rectMin, rectMax, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintNormal);
}

void DrawButtonImage(Ref<Texture2D>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImRect rect) noexcept {
    auto* drawList = ImGui::GetForegroundDrawList();
    if (ImGui::IsItemActive())
        drawList->AddImage((uint64_t)(image->GetRawPointer()), rect.Min, rect.Max, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintPressed);
    else if (ImGui::IsItemHovered())
        drawList->AddImage((uint64_t)(image->GetRawPointer()), rect.Min, rect.Max, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintHovered);
    else
        drawList->AddImage((uint64_t)(image->GetRawPointer()), rect.Min, rect.Max, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintNormal);
}

void RenderWindowOuterBorders(ImGuiWindow* window) {
    struct ImGuiResizeBorderDef {
        ImVec2 InnerDir;
        ImVec2 SegmentN1, SegmentN2;
        float  OuterAngle;
    };
    static constexpr auto resizeBorderDef = std::to_array<ImGuiResizeBorderDef>({
        { ImVec2(1,  0), ImVec2(0, 1), ImVec2(0, 0), IM_PI * 1.00f }, // Left
        { ImVec2(-1,  0), ImVec2(1, 0), ImVec2(1, 1), IM_PI * 0.00f }, // Right
        { ImVec2(0,  1), ImVec2(0, 0), ImVec2(1, 0), IM_PI * 1.50f }, // Up
        { ImVec2(0, -1), ImVec2(1, 1), ImVec2(0, 1), IM_PI * 0.50f }  // Down
    });

    ImGuiContext& g = *GImGui;
    const float rounding = window->WindowRounding;
    constexpr float borderSize = 1.0f; // window->WindowBorderSize;
    if (borderSize > 0.0f && ((window->Flags & ImGuiWindowFlags_NoBackground) == 0))
        window->DrawList->AddRect(window->Pos, { window->Pos.x + window->Size.x,  window->Pos.y + window->Size.y }, ImGui::GetColorU32(ImGuiCol_Border), rounding, 0, borderSize);

    int borderHeld = window->ResizeBorderHeld;
    if (borderHeld != -1) {
        const ImGuiResizeBorderDef& def = resizeBorderDef[borderHeld];
        ImRect borderRect = GetResizeBorderRect(window, borderHeld, rounding, 0.0f);
        ImVec2 p1 = ImLerp(borderRect.Min, borderRect.Max, def.SegmentN1);
        const float offsetX = def.InnerDir.x * rounding;
        const float offsetY = def.InnerDir.y * rounding;
        p1.x += 0.5f + offsetX;
        p1.y += 0.5f + offsetY;

        ImVec2 p2 = ImLerp(borderRect.Min, borderRect.Max, def.SegmentN2);
        p2.x += 0.5f + offsetX;
        p2.y += 0.5f + offsetY;

        window->DrawList->PathArcTo(p1, rounding, def.OuterAngle - IM_PI * 0.25f, def.OuterAngle);
        window->DrawList->PathArcTo(p2, rounding, def.OuterAngle, def.OuterAngle + IM_PI * 0.25f);
        window->DrawList->PathStroke(ImGui::GetColorU32(ImGuiCol_SeparatorActive), 0, ImMax(2.0f, borderSize));
    }
    if (g.Style.FrameBorderSize > 0 && ((window->Flags & ImGuiWindowFlags_NoTitleBar) == 0) && !window->DockIsActive) {
        float y = window->Pos.y + window->TitleBarHeight - 1;
        window->DrawList->AddLine(ImVec2(window->Pos.x + borderSize, y), ImVec2(window->Pos.x + window->Size.x - borderSize, y),
            ImGui::GetColorU32(ImGuiCol_Border), g.Style.FrameBorderSize);
    }
}

}
