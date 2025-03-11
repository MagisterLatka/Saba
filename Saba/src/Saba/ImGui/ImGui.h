#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Renderer/Texture.h"
#include "Saba/Renderer/RenderTarget.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Saba::UI {

SB_CORE void DrawImage(Ref<Texture2D> texture, ImVec2 size);
SB_CORE void DrawImage(Ref<RenderTarget> renderTarget, ImVec2 size);

SB_CORE bool DragFloat(const std::string& label, float& value, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f,
    float columnWidth = 100.0f);
SB_CORE bool DragFloat2(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f,
    float columnWidth = 100.0f);
SB_CORE bool DragFloat3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f,
    float columnWidth = 100.0f);
SB_CORE bool DragFloat4(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f,
    float columnWidth = 100.0f);

SB_CORE bool DragInt(const std::string& label, int& value, int resetValue = 0, int minVal = 0, int maxVal = 0, float speed = 0.0f, float columnWidth = 100.0f);
SB_CORE bool DragInt2(const std::string& label, glm::ivec2& values, int resetValue = 0, int minVal = 0, int maxVal = 0, float speed = 0.0f, float columnWidth = 100.0f);
SB_CORE bool DragInt3(const std::string& label, glm::ivec3& values, int resetValue = 0, int minVal = 0, int maxVal = 0, float speed = 0.0f, float columnWidth = 100.0f);
SB_CORE bool DragInt4(const std::string& label, glm::ivec4& values, int resetValue = 0, int minVal = 0, int maxVal = 0, float speed = 0.0f, float columnWidth = 100.0f);

SB_CORE bool ColorEdit3(const std::string& label, glm::vec3& values, float columnWidth = 100.0f);
SB_CORE bool ColorEdit4(const std::string& label, glm::vec4& values, float columnWidth = 100.0f);

SB_CORE void HelpMarker(const char* desc);

SB_CORE void ShiftCursorX(float x) noexcept;
SB_CORE void ShiftCursorY(float y) noexcept;
SB_CORE void ShiftCursor(float x, float y) noexcept;

SB_CORE ImRect GetItemRect() noexcept;
SB_CORE ImRect RectExpand(const ImRect& rect, float x, float y) noexcept;
SB_CORE ImRect RectMove(const ImRect& rect, float x, float y) noexcept;

bool BeginMenuBar(const ImRect& menuBarRect);
void EndMenuBar();

SB_CORE void DrawButtonImage(Ref<Texture2D>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImVec2 rectMin, ImVec2 rectMax) noexcept;
SB_CORE void DrawButtonImage(Ref<Texture2D>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImRect rect) noexcept;

SB_CORE void RenderWindowOuterBorders(ImGuiWindow* window);

}
