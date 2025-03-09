#include <pch.h>
#include "Input.h"

#include "Saba/Core/Application.h"

namespace Saba {

bool Input::IsKeyPressed(KeyCode key) {
    return Application::Get().GetWindow()->GetKeyboard().IsKeyPressed(static_cast<uint8_t>(key));
}
bool Input::IsMouseButtonPressed(MouseCode button) {
    const auto& mouse = Application::Get().GetWindow()->GetMouse();
    switch (button) {
        case MouseCode::Button0:
            return mouse.IsLeftButtonPressed();
        case MouseCode::Button1:
            return mouse.IsRightButtonPressed();
        case MouseCode::Button2:
        case MouseCode::Button3:
        case MouseCode::Button4:
        case MouseCode::Button5:
        case MouseCode::Button6:
        case MouseCode::Button7:
            return false;
    }
    return false;
}
glm::vec2 Input::GetMousePos() {
    return Application::Get().GetWindow()->GetMouse().GetPos();
}

}
