#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Renderer/Texture.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Saba::UI {

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
