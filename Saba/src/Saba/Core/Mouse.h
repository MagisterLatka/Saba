#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class Mouse {
    friend class LinuxWindow;
public:
    class Event {
    public:
        enum class Type : uint8_t { LPress, LRelease, RPress, RRelease, WheelUp, WheelDown, Move, Enter, Leave, Invalid };
    public:
        SB_CORE Event() noexcept
            : m_Type(Type::Invalid), m_IsLeftPressed(false), m_IsRightPressed(false), m_Pos(0, 0) {}
        SB_CORE Event(Type type, const Mouse& mouse) noexcept
            : m_Type(type), m_IsLeftPressed(mouse.m_IsLeftPressed), m_IsRightPressed(mouse.m_IsRightPressed), m_Pos(mouse.m_Pos) {}

        SB_CORE bool IsValid() const noexcept { return m_Type != Type::Invalid; }
        SB_CORE Type GetType() const noexcept { return m_Type; }
        SB_CORE const glm::ivec2& GetPos() const noexcept { return m_Pos; }
        SB_CORE bool IsLeftButtonPressed() const noexcept { return m_IsLeftPressed; }
        SB_CORE bool IsRightButtonPressed() const noexcept { return m_IsRightPressed; }
    private:
        Type m_Type;
        bool m_IsLeftPressed, m_IsRightPressed;
        glm::ivec2 m_Pos;
    };
public:
    SB_CORE Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;

    SB_CORE glm::ivec2 GetPos() const noexcept { return m_Pos; }
    SB_CORE bool IsInWindow() const noexcept { return m_IsInWindow; }

    SB_CORE bool IsLeftButtonPressed() const noexcept { return m_IsLeftPressed; }
    SB_CORE bool IsRightButtonPressed() const noexcept { return m_IsRightPressed; }

    SB_CORE Mouse::Event ReadEvent() noexcept;
    SB_CORE bool IsEmpty() const noexcept { return m_Buffer.empty(); }

    SB_CORE void Flush() noexcept;
private:
    SB_CORE void OnMouseMove(int x, int y) noexcept;
    SB_CORE void OnMouseLeave() noexcept;
    SB_CORE void OnMouseEnter() noexcept;

    SB_CORE void OnLeftPressed() noexcept;
    SB_CORE void OnLeftReleased() noexcept;
    SB_CORE void OnRightPressed() noexcept;
    SB_CORE void OnRightReleased() noexcept;

    SB_CORE void OnWheelUp() noexcept;
    SB_CORE void OnWheelDown() noexcept;
    SB_CORE void OnWheelDelta(int delta) noexcept;

    SB_CORE void TrimBuffer() noexcept;
private:
    static constexpr uint8_t c_BufferSize = 16u;

    glm::ivec2 m_Pos;
    bool m_IsLeftPressed = false, m_IsRightPressed = false, m_IsInWindow = false;
    int m_WheelDelta = 0;
    std::queue<Event> m_Buffer;
};

} //namespace Saba
