#pragma once

namespace Saba
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define SB_KEY_SPACE           ::Saba::Key::Space
#define SB_KEY_APOSTROPHE      ::Saba::Key::Apostrophe    /* ' */
#define SB_KEY_COMMA           ::Saba::Key::Comma         /* , */
#define SB_KEY_MINUS           ::Saba::Key::Minus         /* - */
#define SB_KEY_PERIOD          ::Saba::Key::Period        /* . */
#define SB_KEY_SLASH           ::Saba::Key::Slash         /* / */
#define SB_KEY_0               ::Saba::Key::D0
#define SB_KEY_1               ::Saba::Key::D1
#define SB_KEY_2               ::Saba::Key::D2
#define SB_KEY_3               ::Saba::Key::D3
#define SB_KEY_4               ::Saba::Key::D4
#define SB_KEY_5               ::Saba::Key::D5
#define SB_KEY_6               ::Saba::Key::D6
#define SB_KEY_7               ::Saba::Key::D7
#define SB_KEY_8               ::Saba::Key::D8
#define SB_KEY_9               ::Saba::Key::D9
#define SB_KEY_SEMICOLON       ::Saba::Key::Semicolon     /* ; */
#define SB_KEY_EQUAL           ::Saba::Key::Equal         /* = */
#define SB_KEY_A               ::Saba::Key::A
#define SB_KEY_B               ::Saba::Key::B
#define SB_KEY_C               ::Saba::Key::C
#define SB_KEY_D               ::Saba::Key::D
#define SB_KEY_E               ::Saba::Key::E
#define SB_KEY_F               ::Saba::Key::F
#define SB_KEY_G               ::Saba::Key::G
#define SB_KEY_H               ::Saba::Key::H
#define SB_KEY_I               ::Saba::Key::I
#define SB_KEY_J               ::Saba::Key::J
#define SB_KEY_K               ::Saba::Key::K
#define SB_KEY_L               ::Saba::Key::L
#define SB_KEY_M               ::Saba::Key::M
#define SB_KEY_N               ::Saba::Key::N
#define SB_KEY_O               ::Saba::Key::O
#define SB_KEY_P               ::Saba::Key::P
#define SB_KEY_Q               ::Saba::Key::Q
#define SB_KEY_R               ::Saba::Key::R
#define SB_KEY_S               ::Saba::Key::S
#define SB_KEY_T               ::Saba::Key::T
#define SB_KEY_U               ::Saba::Key::U
#define SB_KEY_V               ::Saba::Key::V
#define SB_KEY_W               ::Saba::Key::W
#define SB_KEY_X               ::Saba::Key::X
#define SB_KEY_Y               ::Saba::Key::Y
#define SB_KEY_Z               ::Saba::Key::Z
#define SB_KEY_LEFT_BRACKET    ::Saba::Key::LeftBracket   /* [ */
#define SB_KEY_BACKSLASH       ::Saba::Key::Backslash     /* \ */
#define SB_KEY_RIGHT_BRACKET   ::Saba::Key::RightBracket  /* ] */
#define SB_KEY_GRAVE_ACCENT    ::Saba::Key::GraveAccent   /* ` */
#define SB_KEY_WORLD_1         ::Saba::Key::World1        /* non-US #1 */
#define SB_KEY_WORLD_2         ::Saba::Key::World2        /* non-US #2 */

/* Function keys */
#define SB_KEY_ESCAPE          ::Saba::Key::Escape
#define SB_KEY_ENTER           ::Saba::Key::Enter
#define SB_KEY_TAB             ::Saba::Key::Tab
#define SB_KEY_BACKSPACE       ::Saba::Key::Backspace
#define SB_KEY_INSERT          ::Saba::Key::Insert
#define SB_KEY_DELETE          ::Saba::Key::Delete
#define SB_KEY_RIGHT           ::Saba::Key::Right
#define SB_KEY_LEFT            ::Saba::Key::Left
#define SB_KEY_DOWN            ::Saba::Key::Down
#define SB_KEY_UP              ::Saba::Key::Up
#define SB_KEY_PAGE_UP         ::Saba::Key::PageUp
#define SB_KEY_PAGE_DOWN       ::Saba::Key::PageDown
#define SB_KEY_HOME            ::Saba::Key::Home
#define SB_KEY_END             ::Saba::Key::End
#define SB_KEY_CAPS_LOCK       ::Saba::Key::CapsLock
#define SB_KEY_SCROLL_LOCK     ::Saba::Key::ScrollLock
#define SB_KEY_NUM_LOCK        ::Saba::Key::NumLock
#define SB_KEY_PRINT_SCREEN    ::Saba::Key::PrintScreen
#define SB_KEY_PAUSE           ::Saba::Key::Pause
#define SB_KEY_F1              ::Saba::Key::F1
#define SB_KEY_F2              ::Saba::Key::F2
#define SB_KEY_F3              ::Saba::Key::F3
#define SB_KEY_F4              ::Saba::Key::F4
#define SB_KEY_F5              ::Saba::Key::F5
#define SB_KEY_F6              ::Saba::Key::F6
#define SB_KEY_F7              ::Saba::Key::F7
#define SB_KEY_F8              ::Saba::Key::F8
#define SB_KEY_F9              ::Saba::Key::F9
#define SB_KEY_F10             ::Saba::Key::F10
#define SB_KEY_F11             ::Saba::Key::F11
#define SB_KEY_F12             ::Saba::Key::F12
#define SB_KEY_F13             ::Saba::Key::F13
#define SB_KEY_F14             ::Saba::Key::F14
#define SB_KEY_F15             ::Saba::Key::F15
#define SB_KEY_F16             ::Saba::Key::F16
#define SB_KEY_F17             ::Saba::Key::F17
#define SB_KEY_F18             ::Saba::Key::F18
#define SB_KEY_F19             ::Saba::Key::F19
#define SB_KEY_F20             ::Saba::Key::F20
#define SB_KEY_F21             ::Saba::Key::F21
#define SB_KEY_F22             ::Saba::Key::F22
#define SB_KEY_F23             ::Saba::Key::F23
#define SB_KEY_F24             ::Saba::Key::F24
#define SB_KEY_F25             ::Saba::Key::F25

/* Keypad */
#define SB_KEY_KP_0            ::Saba::Key::KP0
#define SB_KEY_KP_1            ::Saba::Key::KP1
#define SB_KEY_KP_2            ::Saba::Key::KP2
#define SB_KEY_KP_3            ::Saba::Key::KP3
#define SB_KEY_KP_4            ::Saba::Key::KP4
#define SB_KEY_KP_5            ::Saba::Key::KP5
#define SB_KEY_KP_6            ::Saba::Key::KP6
#define SB_KEY_KP_7            ::Saba::Key::KP7
#define SB_KEY_KP_8            ::Saba::Key::KP8
#define SB_KEY_KP_9            ::Saba::Key::KP9
#define SB_KEY_KP_DECIMAL      ::Saba::Key::KPDecimal
#define SB_KEY_KP_DIVIDE       ::Saba::Key::KPDivide
#define SB_KEY_KP_MULTIPLY     ::Saba::Key::KPMultiply
#define SB_KEY_KP_SUBTRACT     ::Saba::Key::KPSubtract
#define SB_KEY_KP_ADD          ::Saba::Key::KPAdd
#define SB_KEY_KP_ENTER        ::Saba::Key::KPEnter
#define SB_KEY_KP_EQUAL        ::Saba::Key::KPEqual

#define SB_KEY_LEFT_SHIFT      ::Saba::Key::LeftShift
#define SB_KEY_LEFT_CONTROL    ::Saba::Key::LeftControl
#define SB_KEY_LEFT_ALT        ::Saba::Key::LeftAlt
#define SB_KEY_LEFT_SUPER      ::Saba::Key::LeftSuper
#define SB_KEY_RIGHT_SHIFT     ::Saba::Key::RightShift
#define SB_KEY_RIGHT_CONTROL   ::Saba::Key::RightControl
#define SB_KEY_RIGHT_ALT       ::Saba::Key::RightAlt
#define SB_KEY_RIGHT_SUPER     ::Saba::Key::RightSuper
#define SB_KEY_MENU            ::Saba::Key::Menu
