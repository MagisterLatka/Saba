#pragma once

namespace Saba {

enum class KeyCode : uint8_t {
    Space = 0x20u,
    Apostrophe = 0xC0u, /* ' */
    Comma = 0xBCu, /* u, */
    Minus = 0xBDu, /* - */
    Period = 0xBEu, /* . */
    Slash = 0xBFu, /* / */

    D0 = 0x30u, /* 0 */
    D1 = 0x31u, /* 1 */
    D2 = 0x32u, /* 2 */
    D3 = 0x33u, /* 3 */
    D4 = 0x34u, /* 4 */
    D5 = 0x35u, /* 5 */
    D6 = 0x36u, /* 6 */
    D7 = 0x37u, /* 7 */
    D8 = 0x38u, /* 8 */
    D9 = 0x39u, /* 9 */

    Semicolon = 0xBAu, /* ; */
    Equal = 0xBBu, /* = */

    A = 0x41u,
    B = 0x42u,
    C = 0x43u,
    D = 0x44u,
    E = 0x45u,
    F = 0x46u,
    G = 0x47u,
    H = 0x48u,
    I = 0x49u,
    J = 0x4Au,
    K = 0x4Bu,
    L = 0x4Cu,
    M = 0x4Du,
    N = 0x4Eu,
    O = 0x4Fu,
    P = 0x50u,
    Q = 0x51u,
    R = 0x52u,
    S = 0x53u,
    T = 0x54u,
    U = 0x55u,
    V = 0x56u,
    W = 0x57u,
    X = 0x58u,
    Y = 0x59u,
    Z = 0x5Au,

    LeftBracket = 0xDBu,  /* [ */
    Backslash = 0xDCu,  /* \ */
    RightBracket = 0xDDu,  /* ] */
    GraveAccent = 0xDEu,  /* ` */

    Escape = 0x1Bu,
    Enter = 0x0Du,
    Tab = 0x09u,
    Backspace = 0x08u,
    Insert = 0x2Du,
    Delete = 0x2Eu,
    Right = 0x27u,
    Left = 0x25u,
    Down = 0x28u,
    Up = 0x26u,
    PageUp = 0x21u,
    PageDown = 0x22u,
    Home = 0x24u,
    End = 0x23u,
    CapsLock = 0x14u,
    ScrollLock = 0x91u,
    NumLock = 0x90u,
    PrintScreen = 0x2Cu,
    Pause = 0x13u,
    F1 = 0x70u,
    F2 = 0x71u,
    F3 = 0x72u,
    F4 = 0x73u,
    F5 = 0x74u,
    F6 = 0x75u,
    F7 = 0x76u,
    F8 = 0x77u,
    F9 = 0x78u,
    F10 = 0x79u,
    F11 = 0x7Au,
    F12 = 0x7Bu,
    F13 = 0x7Cu,
    F14 = 0x7Du,
    F15 = 0x7Eu,
    F16 = 0x7Fu,
    F17 = 0x80u,
    F18 = 0x81u,
    F19 = 0x82u,
    F20 = 0x83u,
    F21 = 0x84u,
    F22 = 0x85u,
    F23 = 0x86u,
    F24 = 0x87u,

    /* Keypad */
    KP0 = 0x60u,
    KP1 = 0x61u,
    KP2 = 0x62u,
    KP3 = 0x63u,
    KP4 = 0x64u,
    KP5 = 0x65u,
    KP6 = 0x66u,
    KP7 = 0x67u,
    KP8 = 0x68u,
    KP9 = 0x69u,
    KPDecimal = 0x6Eu,
    KPDivide = 0x6Fu,
    KPMultiply = 0x6Au,
    KPSubtract = 0x6Du,
    KPAdd = 0x6Bu,

    LeftShift = 0xA0u,
    LeftControl = 0xA2u,
    LeftAlt = 0xA4u,
    RightShift = 0xA1u,
    RightControl = 0xA3u,
    RightAlt = 0xA5u,
    Menu = 0x5Bu
};

inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode) noexcept {
    os << static_cast<uint32_t>(keyCode);
    return os;
}
inline uint8_t GetVKCode(KeyCode keyCode) noexcept {
    return static_cast<uint8_t>(keyCode);
}
inline uint16_t GetGLFWCode(KeyCode keycode) noexcept {
    switch (keycode)
    {
        case KeyCode::Space:        return 0x20u;
        case KeyCode::Apostrophe:   return 0x27u;
        case KeyCode::Comma:        return 0x2Cu;
        case KeyCode::Minus:        return 0x2Du;
        case KeyCode::Period:       return 0x2Eu;
        case KeyCode::Slash:        return 0x2Fu;

        case KeyCode::D0:           return 0x30u;
        case KeyCode::D1:           return 0x31u;
        case KeyCode::D2:           return 0x32u;
        case KeyCode::D3:           return 0x33u;
        case KeyCode::D4:           return 0x34u;
        case KeyCode::D5:           return 0x35u;
        case KeyCode::D6:           return 0x36u;
        case KeyCode::D7:           return 0x37u;
        case KeyCode::D8:           return 0x38u;
        case KeyCode::D9:           return 0x39u;

        case KeyCode::Semicolon:    return 0x3Bu;
        case KeyCode::Equal:        return 0x3Du;

        case KeyCode::A:            return 0x41u;
        case KeyCode::B:            return 0x42u;
        case KeyCode::C:            return 0x43u;
        case KeyCode::D:            return 0x44u;
        case KeyCode::E:            return 0x45u;
        case KeyCode::F:            return 0x46u;
        case KeyCode::G:            return 0x47u;
        case KeyCode::H:            return 0x48u;
        case KeyCode::I:            return 0x49u;
        case KeyCode::J:            return 0x4Au;
        case KeyCode::K:            return 0x4Bu;
        case KeyCode::L:            return 0x4Cu;
        case KeyCode::M:            return 0x4Du;
        case KeyCode::N:            return 0x4Eu;
        case KeyCode::O:            return 0x4Fu;
        case KeyCode::P:            return 0x50u;
        case KeyCode::Q:            return 0x51u;
        case KeyCode::R:            return 0x52u;
        case KeyCode::S:            return 0x53u;
        case KeyCode::T:            return 0x54u;
        case KeyCode::U:            return 0x55u;
        case KeyCode::V:            return 0x56u;
        case KeyCode::W:            return 0x57u;
        case KeyCode::X:            return 0x58u;
        case KeyCode::Y:            return 0x59u;
        case KeyCode::Z:            return 0x5Au;

        case KeyCode::LeftBracket:  return 0x5Bu;
        case KeyCode::Backslash:    return 0x5Cu;
        case KeyCode::RightBracket: return 0x5Du;
        case KeyCode::GraveAccent:  return 0x60u;

        case KeyCode::Escape:       return 0x100u;
        case KeyCode::Enter:        return 0x101u;
        case KeyCode::Tab:          return 0x102u;
        case KeyCode::Backspace:    return 0x103u;
        case KeyCode::Insert:       return 0x104u;
        case KeyCode::Delete:       return 0x105u;
        case KeyCode::Right:        return 0x106u;
        case KeyCode::Left:         return 0x107u;
        case KeyCode::Down:         return 0x108u;
        case KeyCode::Up:           return 0x109u;
        case KeyCode::PageUp:       return 0x10Au;
        case KeyCode::PageDown:     return 0x10Bu;
        case KeyCode::Home:         return 0x10Cu;
        case KeyCode::End:          return 0x10Du;
        case KeyCode::CapsLock:     return 0x118u;
        case KeyCode::ScrollLock:   return 0x119u;
        case KeyCode::NumLock:      return 0x11Au;
        case KeyCode::PrintScreen:  return 0x11Bu;
        case KeyCode::Pause:        return 0x11Cu;
        case KeyCode::F1:           return 0x122u;
        case KeyCode::F2:           return 0x123u;
        case KeyCode::F3:           return 0x124u;
        case KeyCode::F4:           return 0x125u;
        case KeyCode::F5:           return 0x126u;
        case KeyCode::F6:           return 0x127u;
        case KeyCode::F7:           return 0x128u;
        case KeyCode::F8:           return 0x129u;
        case KeyCode::F9:           return 0x12Au;
        case KeyCode::F10:          return 0x12Bu;
        case KeyCode::F11:          return 0x12Cu;
        case KeyCode::F12:          return 0x12Du;
        case KeyCode::F13:          return 0x12Eu;
        case KeyCode::F14:          return 0x12Fu;
        case KeyCode::F15:          return 0x130u;
        case KeyCode::F16:          return 0x131u;
        case KeyCode::F17:          return 0x132u;
        case KeyCode::F18:          return 0x133u;
        case KeyCode::F19:          return 0x134u;
        case KeyCode::F20:          return 0x135u;
        case KeyCode::F21:          return 0x136u;
        case KeyCode::F22:          return 0x137u;
        case KeyCode::F23:          return 0x138u;
        case KeyCode::F24:          return 0x139u;

        /* Keypad */
        case KeyCode::KP0:          return 0x140u;
        case KeyCode::KP1:          return 0x141u;
        case KeyCode::KP2:          return 0x142u;
        case KeyCode::KP3:          return 0x143u;
        case KeyCode::KP4:          return 0x144u;
        case KeyCode::KP5:          return 0x145u;
        case KeyCode::KP6:          return 0x146u;
        case KeyCode::KP7:          return 0x147u;
        case KeyCode::KP8:          return 0x148u;
        case KeyCode::KP9:          return 0x149u;
        case KeyCode::KPDecimal:    return 0x14Au;
        case KeyCode::KPDivide:     return 0x14Bu;
        case KeyCode::KPMultiply:   return 0x14Cu;
        case KeyCode::KPSubtract:   return 0x14Du;
        case KeyCode::KPAdd:        return 0x14Eu;

        case KeyCode::LeftShift:    return 0x154u;
        case KeyCode::LeftControl:  return 0x155u;
        case KeyCode::LeftAlt:      return 0x156u;
        case KeyCode::RightShift:   return 0x158u;
        case KeyCode::RightControl: return 0x159u;
        case KeyCode::RightAlt:     return 0x15Au;
        case KeyCode::Menu:         return 0x15Cu;
        default:                    return 0;
    }
}
inline KeyCode GetKeyCodeFromGLFW(int key) {
    switch (key)
    {
        case 32: return KeyCode::Space;
        case 39: return KeyCode::Apostrophe;
        case 44: return KeyCode::Comma;
        case 45: return KeyCode::Minus;
        case 46: return KeyCode::Period;
        case 47: return KeyCode::Slash;
        case 48: return KeyCode::D0;
        case 49: return KeyCode::D1;
        case 50: return KeyCode::D2;
        case 51: return KeyCode::D3;
        case 52: return KeyCode::D4;
        case 53: return KeyCode::D5;
        case 54: return KeyCode::D6;
        case 55: return KeyCode::D7;
        case 56: return KeyCode::D8;
        case 57: return KeyCode::D9;
        case 59: return KeyCode::Semicolon;
        case 61: return KeyCode::Equal;
        case 65: return KeyCode::A;
        case 66: return KeyCode::B;
        case 67: return KeyCode::C;
        case 68: return KeyCode::D;
        case 69: return KeyCode::E;
        case 70: return KeyCode::F;
        case 71: return KeyCode::G;
        case 72: return KeyCode::H;
        case 73: return KeyCode::I;
        case 74: return KeyCode::J;
        case 75: return KeyCode::K;
        case 76: return KeyCode::L;
        case 77: return KeyCode::M;
        case 78: return KeyCode::N;
        case 79: return KeyCode::O;
        case 80: return KeyCode::P;
        case 81: return KeyCode::Q;
        case 82: return KeyCode::R;
        case 83: return KeyCode::S;
        case 84: return KeyCode::T;
        case 85: return KeyCode::U;
        case 86: return KeyCode::V;
        case 87: return KeyCode::W;
        case 88: return KeyCode::X;
        case 89: return KeyCode::Y;
        case 90: return KeyCode::Z;
        case 91: return KeyCode::LeftBracket;
        case 92: return KeyCode::Backslash;
        case 93: return KeyCode::RightBracket;
        case 96: return KeyCode::GraveAccent;
        case 256: return KeyCode::Escape;
        case 257: return KeyCode::Enter;
        case 258: return KeyCode::Tab;
        case 259: return KeyCode::Backspace;
        case 260: return KeyCode::Insert;
        case 261: return KeyCode::Delete;
        case 262: return KeyCode::Right;
        case 263: return KeyCode::Left;
        case 264: return KeyCode::Down;
        case 265: return KeyCode::Up;
        case 266: return KeyCode::PageUp;
        case 267: return KeyCode::PageDown;
        case 268: return KeyCode::Home;
        case 269: return KeyCode::End;
        case 280: return KeyCode::CapsLock;
        case 281: return KeyCode::ScrollLock;
        case 282: return KeyCode::NumLock;
        case 283: return KeyCode::PrintScreen;
        case 284: return KeyCode::Pause;
        case 290: return KeyCode::F1;
        case 291: return KeyCode::F2;
        case 292: return KeyCode::F3;
        case 293: return KeyCode::F4;
        case 294: return KeyCode::F5;
        case 295: return KeyCode::F6;
        case 296: return KeyCode::F7;
        case 297: return KeyCode::F8;
        case 298: return KeyCode::F9;
        case 299: return KeyCode::F10;
        case 300: return KeyCode::F11;
        case 301: return KeyCode::F12;
        case 302: return KeyCode::F13;
        case 303: return KeyCode::F14;
        case 304: return KeyCode::F15;
        case 305: return KeyCode::F16;
        case 306: return KeyCode::F17;
        case 307: return KeyCode::F18;
        case 308: return KeyCode::F19;
        case 309: return KeyCode::F20;
        case 310: return KeyCode::F21;
        case 311: return KeyCode::F22;
        case 312: return KeyCode::F23;
        case 313: return KeyCode::F24;
        case 320: return KeyCode::KP0;
        case 321: return KeyCode::KP1;
        case 322: return KeyCode::KP2;
        case 323: return KeyCode::KP3;
        case 324: return KeyCode::KP4;
        case 325: return KeyCode::KP5;
        case 326: return KeyCode::KP6;
        case 327: return KeyCode::KP7;
        case 328: return KeyCode::KP8;
        case 329: return KeyCode::KP9;
        case 330: return KeyCode::KPDecimal;
        case 331: return KeyCode::KPDivide;
        case 332: return KeyCode::KPMultiply;
        case 333: return KeyCode::KPSubtract;
        case 334: return KeyCode::KPAdd;
        case 340: return KeyCode::LeftShift;
        case 341: return KeyCode::LeftControl;
        case 342: return KeyCode::LeftAlt;
        case 344: return KeyCode::RightShift;
        case 345: return KeyCode::RightControl;
        case 346: return KeyCode::RightAlt;
        case 348: return KeyCode::Menu;
        default:  return KeyCode::D0;
    }
}

} //namespace Saba

#define SB_KEY_SPACE            ::Saba::KeyCode::Space
#define SB_KEY_APOSTROPHE       ::Saba::KeyCode::Apostrophe    /* ' */
#define SB_KEY_COMMA            ::Saba::KeyCode::Comma         /* , */
#define SB_KEY_MINUS            ::Saba::KeyCode::Minus         /* - */
#define SB_KEY_PERIOD           ::Saba::KeyCode::Period        /* . */
#define SB_KEY_SLASH            ::Saba::KeyCode::Slash         /* / */
#define SB_KEY_0                ::Saba::KeyCode::D0
#define SB_KEY_1                ::Saba::KeyCode::D1
#define SB_KEY_2                ::Saba::KeyCode::D2
#define SB_KEY_3                ::Saba::KeyCode::D3
#define SB_KEY_4                ::Saba::KeyCode::D4
#define SB_KEY_5                ::Saba::KeyCode::D5
#define SB_KEY_6                ::Saba::KeyCode::D6
#define SB_KEY_7                ::Saba::KeyCode::D7
#define SB_KEY_8                ::Saba::KeyCode::D8
#define SB_KEY_9                ::Saba::KeyCode::D9
#define SB_KEY_SEMICOLON        ::Saba::KeyCode::Semicolon     /* ; */
#define SB_KEY_EQUAL            ::Saba::KeyCode::Equal         /* = */
#define SB_KEY_A                ::Saba::KeyCode::A
#define SB_KEY_B                ::Saba::KeyCode::B
#define SB_KEY_C                ::Saba::KeyCode::C
#define SB_KEY_D                ::Saba::KeyCode::D
#define SB_KEY_E                ::Saba::KeyCode::E
#define SB_KEY_F                ::Saba::KeyCode::F
#define SB_KEY_G                ::Saba::KeyCode::G
#define SB_KEY_H                ::Saba::KeyCode::H
#define SB_KEY_I                ::Saba::KeyCode::I
#define SB_KEY_J                ::Saba::KeyCode::J
#define SB_KEY_K                ::Saba::KeyCode::K
#define SB_KEY_L                ::Saba::KeyCode::L
#define SB_KEY_M                ::Saba::KeyCode::M
#define SB_KEY_N                ::Saba::KeyCode::N
#define SB_KEY_O                ::Saba::KeyCode::O
#define SB_KEY_P                ::Saba::KeyCode::P
#define SB_KEY_Q                ::Saba::KeyCode::Q
#define SB_KEY_R                ::Saba::KeyCode::R
#define SB_KEY_S                ::Saba::KeyCode::S
#define SB_KEY_T                ::Saba::KeyCode::T
#define SB_KEY_U                ::Saba::KeyCode::U
#define SB_KEY_V                ::Saba::KeyCode::V
#define SB_KEY_W                ::Saba::KeyCode::W
#define SB_KEY_X                ::Saba::KeyCode::X
#define SB_KEY_Y                ::Saba::KeyCode::Y
#define SB_KEY_Z                ::Saba::KeyCode::Z
#define SB_KEY_LEFT_BRACKET     ::Saba::KeyCode::LeftBracket   /* [ */
#define SB_KEY_BACKSLASH        ::Saba::KeyCode::Backslash     /* \ */
#define SB_KEY_RIGHT_BRACKET    ::Saba::KeyCode::RightBracket  /* ] */
#define SB_KEY_GRAVE_ACCENT     ::Saba::KeyCode::GraveAccent   /* ` */

#define SB_KEY_ESCAPE           ::Saba::KeyCode::Escape
#define SB_KEY_ENTER            ::Saba::KeyCode::Enter
#define SB_KEY_TAB              ::Saba::KeyCode::Tab
#define SB_KEY_BACKSPACE        ::Saba::KeyCode::Backspace
#define SB_KEY_INSERT           ::Saba::KeyCode::Insert
#define SB_KEY_DELETE           ::Saba::KeyCode::Delete
#define SB_KEY_RIGHT            ::Saba::KeyCode::Right
#define SB_KEY_LEFT             ::Saba::KeyCode::Left
#define SB_KEY_DOWN             ::Saba::KeyCode::Down
#define SB_KEY_UP               ::Saba::KeyCode::Up
#define SB_KEY_PAGE_UP          ::Saba::KeyCode::PageUp
#define SB_KEY_PAGE_DOWN        ::Saba::KeyCode::PageDown
#define SB_KEY_HOME             ::Saba::KeyCode::Home
#define SB_KEY_END              ::Saba::KeyCode::End
#define SB_KEY_CAPS_LOCK        ::Saba::KeyCode::CapsLock
#define SB_KEY_SCROLL_LOCK      ::Saba::KeyCode::ScrollLock
#define SB_KEY_NUM_LOCK         ::Saba::KeyCode::NumLock
#define SB_KEY_PRINT_SCREEN     ::Saba::KeyCode::PrintScreen
#define SB_KEY_PAUSE            ::Saba::KeyCode::Pause
#define SB_KEY_F1               ::Saba::KeyCode::F1
#define SB_KEY_F2               ::Saba::KeyCode::F2
#define SB_KEY_F3               ::Saba::KeyCode::F3
#define SB_KEY_F4               ::Saba::KeyCode::F4
#define SB_KEY_F5               ::Saba::KeyCode::F5
#define SB_KEY_F6               ::Saba::KeyCode::F6
#define SB_KEY_F7               ::Saba::KeyCode::F7
#define SB_KEY_F8               ::Saba::KeyCode::F8
#define SB_KEY_F9               ::Saba::KeyCode::F9
#define SB_KEY_F10              ::Saba::KeyCode::F10
#define SB_KEY_F11              ::Saba::KeyCode::F11
#define SB_KEY_F12              ::Saba::KeyCode::F12
#define SB_KEY_F13              ::Saba::KeyCode::F13
#define SB_KEY_F14              ::Saba::KeyCode::F14
#define SB_KEY_F15              ::Saba::KeyCode::F15
#define SB_KEY_F16              ::Saba::KeyCode::F16
#define SB_KEY_F17              ::Saba::KeyCode::F17
#define SB_KEY_F18              ::Saba::KeyCode::F18
#define SB_KEY_F19              ::Saba::KeyCode::F19
#define SB_KEY_F20              ::Saba::KeyCode::F20
#define SB_KEY_F21              ::Saba::KeyCode::F21
#define SB_KEY_F22              ::Saba::KeyCode::F22
#define SB_KEY_F23              ::Saba::KeyCode::F23
#define SB_KEY_F24              ::Saba::KeyCode::F24
#define SB_KEY_F25              ::Saba::KeyCode::F25

#define SO_FKEY_KP_0            ::Saba::KeyCode::KP0
#define SO_FKEY_KP_1            ::Saba::KeyCode::KP1
#define SO_FKEY_KP_2            ::Saba::KeyCode::KP2
#define SB_KEY_KP_3             ::Saba::KeyCode::KP3
#define SB_KEY_KP_4             ::Saba::KeyCode::KP4
#define SB_KEY_KP_5             ::Saba::KeyCode::KP5
#define SB_KEY_KP_6             ::Saba::KeyCode::KP6
#define SB_KEY_KP_7             ::Saba::KeyCode::KP7
#define SB_KEY_KP_8             ::Saba::KeyCode::KP8
#define SB_KEY_KP_9             ::Saba::KeyCode::KP9
#define SB_KEY_KP_DECIMAL       ::Saba::KeyCode::KPDecimal
#define SB_KEY_KP_DIVIDE        ::Saba::KeyCode::KPDivide
#define SB_KEY_KP_MULTIPLY      ::Saba::KeyCode::KPMultiply
#define SB_KEY_KP_SUBTRACT      ::Saba::KeyCode::KPSubtract
#define SB_KEY_KP_ADD           ::Saba::KeyCode::KPAdd

#define SB_KEY_LEFT_SHIFT       ::Saba::KeyCode::LeftShift
#define SB_KEY_LEFT_CONTROL     ::Saba::KeyCode::LeftControl
#define SB_KEY_LEFT_ALT         ::Saba::KeyCode::LeftAlt
#define SB_KEY_RIGHT_SHIFT      ::Saba::KeyCode::RightShift
#define SB_KEY_RIGHT_CONTROL    ::Saba::KeyCode::RightControl
#define SB_KEY_RIGHT_ALT        ::Saba::KeyCode::RightAlt
#define SB_KEY_MENU             ::Saba::KeyCode::Menu
