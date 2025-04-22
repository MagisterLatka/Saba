#include <pch.h>
#include "Input.h"

#include "Saba/Core/Application.h"

namespace Saba {

bool Input::IsKeyPressed(KeyCode key) noexcept {
    return Application::Get().GetWindow()->GetKeyboard().IsKeyPressed(static_cast<uint8_t>(key));
}
bool Input::IsMouseButtonPressed(MouseCode button) noexcept{
    const auto& mouse = Application::Get().GetWindow()->GetMouse();
    switch (button) {
        case MouseCode::Button0:
            return mouse.IsLeftButtonPressed();
        case MouseCode::Button1:
            return mouse.IsRightButtonPressed();
        default:
            return false;
    }
    return false;
}
glm::vec2 Input::GetMousePos() noexcept {
    return Application::Get().GetWindow()->GetMouse().GetPos();
}

} //namespace Saba
