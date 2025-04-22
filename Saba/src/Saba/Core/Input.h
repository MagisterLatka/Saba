#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Core/KeyCodes.h"
#include "Saba/Core/MouseCodes.h"

namespace Saba {

class SB_CORE Input {
public:
    static bool IsKeyPressed(KeyCode key) noexcept;
    static bool IsMouseButtonPressed(MouseCode button) noexcept;
    static glm::vec2 GetMousePos() noexcept;
};

} //namespace Saba
