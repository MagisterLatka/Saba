#pragma once

namespace Saba {

enum class MouseCode : uint16_t
{
    Button0 = 0u,
    Button1 = 1u,
    Button2 = 2u,
    Button3 = 3u,
    Button4 = 4u,
    Button5 = 5u,
    Button6 = 6u,
    Button7 = 7u,

    ButtonLast = Button7,
    ButtonLeft = Button0,
    ButtonRight = Button1,
    ButtonMiddle = Button2
};

inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode) noexcept
{
    os << static_cast<uint32_t>(mouseCode);
    return os;
}

} //namespace Saba

#define SB_MOUSE_BUTTON_0        ::Saba::MouseCode::Button0
#define SB_MOUSE_BUTTON_1        ::Saba::MouseCode::Button1
#define SB_MOUSE_BUTTON_2        ::Saba::MouseCode::Button2
#define SB_MOUSE_BUTTON_3        ::Saba::MouseCode::Button3
#define SB_MOUSE_BUTTON_4        ::Saba::MouseCode::Button4
#define SB_MOUSE_BUTTON_5        ::Saba::MouseCode::Button5
#define SB_MOUSE_BUTTON_6        ::Saba::MouseCode::Button6
#define SB_MOUSE_BUTTON_7        ::Saba::MouseCode::Button7
#define SB_MOUSE_BUTTON_LAST     ::Saba::MouseCode::ButtonLast
#define SB_MOUSE_BUTTON_LEFT     ::Saba::MouseCode::ButtonLeft
#define SB_MOUSE_BUTTON_RIGHT    ::Saba::MouseCode::ButtonRight
#define SB_MOUSE_BUTTON_MIDDLE   ::Saba::MouseCode::ButtonMiddle
