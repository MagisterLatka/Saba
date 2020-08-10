#pragma once

namespace Saba
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define SB_MOUSE_BUTTON_0      ::Saba::Mouse::Button0
#define SB_MOUSE_BUTTON_1      ::Saba::Mouse::Button1
#define SB_MOUSE_BUTTON_2      ::Saba::Mouse::Button2
#define SB_MOUSE_BUTTON_3      ::Saba::Mouse::Button3
#define SB_MOUSE_BUTTON_4      ::Saba::Mouse::Button4
#define SB_MOUSE_BUTTON_5      ::Saba::Mouse::Button5
#define SB_MOUSE_BUTTON_6      ::Saba::Mouse::Button6
#define SB_MOUSE_BUTTON_7      ::Saba::Mouse::Button7
#define SB_MOUSE_BUTTON_LAST   ::Saba::Mouse::ButtonLast
#define SB_MOUSE_BUTTON_LEFT   ::Saba::Mouse::ButtonLeft
#define SB_MOUSE_BUTTON_RIGHT  ::Saba::Mouse::ButtonRight
#define SB_MOUSE_BUTTON_MIDDLE ::Saba::Mouse::ButtonMiddle
