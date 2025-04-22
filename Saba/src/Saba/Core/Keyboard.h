#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class Keyboard {
    friend class LinuxWindow;
    friend class WindowsWindow;
public:
    class Event {
    public:
        enum class Type : uint8_t { Pressed, Released, Invalid };
    public:
        Event() noexcept
            : m_Type(Type::Invalid), m_Code(0u) {}
        Event(Type type, uint8_t code) noexcept
            : m_Type(type), m_Code(code) {}

        bool IsPressed() const noexcept { return m_Type == Type::Pressed; }
        bool IsReleased() const noexcept { return m_Type == Type::Released; }
        bool IsValid() const noexcept { return m_Type != Type::Invalid; }
        uint8_t GetCode() const noexcept { return m_Code; }
    private:
        Type m_Type;
        uint8_t m_Code;
    };
public:
    SB_CORE Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    bool IsKeyPressed(uint8_t keycode) const noexcept { return m_KeyStates[keycode]; }
    SB_CORE Event ReadKey() noexcept;
    bool IsKeyBufferEmpty() const noexcept { return m_KeyBuffer.empty(); }
    SB_CORE void FlushKey() noexcept;

    SB_CORE char ReadChar() noexcept;
    bool IsCharBufferEmpty() const noexcept { return m_CharBuffer.empty(); }
    SB_CORE void FlushChar() noexcept;

    SB_CORE void Flush() noexcept;

    void EnableAutorepeat() noexcept { m_Autorepeat = true; }
    void DisableAutorepeat() noexcept { m_Autorepeat = false; }
    bool IsAutorepeatEnabled() const noexcept { return m_Autorepeat; }
private:
    void OnKeyPressed(uint8_t keycode) noexcept;
    void OnKeyReleased(uint8_t keycode) noexcept;
    void OnChar(char character) noexcept;
    void ClearState() noexcept;
    template<typename T>
    static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
    static constexpr uint32_t c_NKeys = 256u;
    static constexpr uint32_t c_BufferSize = 16u;

    bool m_Autorepeat = false;
    std::bitset<c_NKeys> m_KeyStates;
    std::queue<Event> m_KeyBuffer;
    std::queue<char> m_CharBuffer;
};

} //namespace Saba
