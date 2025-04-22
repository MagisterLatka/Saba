#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class Mouse {
    friend class LinuxWindow;
    friend class WindowsWindow;
public:
    class Event {
    public:
        enum class Type : uint8_t { LPress, LRelease, RPress, RRelease, WheelUp, WheelDown, Move, Enter, Leave, Invalid };
    public:
        Event() noexcept
            : m_Type(Type::Invalid), m_IsLeftPressed(false), m_IsRightPressed(false), m_Pos(0, 0) {}
        Event(Type type, const Mouse& mouse) noexcept
            : m_Type(type), m_IsLeftPressed(mouse.m_IsLeftPressed), m_IsRightPressed(mouse.m_IsRightPressed), m_Pos(mouse.m_Pos) {}

        bool IsValid() const noexcept { return m_Type != Type::Invalid; }
        Type GetType() const noexcept { return m_Type; }
        const glm::ivec2& GetPos() const noexcept { return m_Pos; }
        bool IsLeftButtonPressed() const noexcept { return m_IsLeftPressed; }
        bool IsRightButtonPressed() const noexcept { return m_IsRightPressed; }
    private:
        Type m_Type;
        bool m_IsLeftPressed, m_IsRightPressed;
        glm::ivec2 m_Pos;
    };
public:
    Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;

    glm::ivec2 GetPos() const noexcept { return m_Pos; }
    bool IsInWindow() const noexcept { return m_IsInWindow; }

    bool IsLeftButtonPressed() const noexcept { return m_IsLeftPressed; }
    bool IsRightButtonPressed() const noexcept { return m_IsRightPressed; }

    SB_CORE Mouse::Event ReadEvent() noexcept;
    bool IsEmpty() const noexcept { return m_Buffer.empty(); }

    SB_CORE void Flush() noexcept;
private:
    void OnMouseMove(int x, int y) noexcept;
    void OnMouseLeave() noexcept;
    void OnMouseEnter() noexcept;

    void OnLeftPressed() noexcept;
    void OnLeftReleased() noexcept;
    void OnRightPressed() noexcept;
    void OnRightReleased() noexcept;

    void OnWheelUp() noexcept;
    void OnWheelDown() noexcept;
    void OnWheelDelta(int delta) noexcept;

    void TrimBuffer() noexcept;
private:
    static constexpr uint8_t c_BufferSize = 16u;

    glm::ivec2 m_Pos;
    bool m_IsLeftPressed = false, m_IsRightPressed = false, m_IsInWindow = false;
    int m_WheelDelta = 0;
    std::queue<Event> m_Buffer;
};

} //namespace Saba
