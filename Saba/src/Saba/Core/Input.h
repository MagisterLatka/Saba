#pragma once

#include "Saba/Core/Core.h"
#include "Saba/Core/KeyCodes.h"
#include "Saba/Core/MouseCodes.h"

namespace Saba {

class SB_CORE Input {
public:
    static bool IsKeyPressed(KeyCode key);
    static bool IsMouseButtonPressed(MouseCode button);
    static glm::vec2 GetMousePos();
};

}
