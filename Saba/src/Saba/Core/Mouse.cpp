#include <pch.h>
#include "Mouse.h"

namespace Saba {

Mouse::Event Mouse::ReadEvent() noexcept {
    if (!m_Buffer.empty()) {
        Event e = m_Buffer.front();
        m_Buffer.pop();
        return e;
    }
    return {};
}
void Mouse::Flush() noexcept {
    m_Buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y) noexcept {
    m_Pos = { x, y };
    m_Buffer.emplace(Event::Type::Move, *this);
    TrimBuffer();
}
void Mouse::OnMouseLeave() noexcept {
    m_IsInWindow = false;
    m_Buffer.emplace(Event::Type::Leave, *this);
    TrimBuffer();
}
void Mouse::OnMouseEnter() noexcept {
    m_IsInWindow = true;
    m_Buffer.emplace(Event::Type::Enter, *this);
    TrimBuffer();
}

void Mouse::OnLeftPressed() noexcept {
    m_IsLeftPressed = true;
    m_Buffer.emplace(Event::Type::LPress, *this);
    TrimBuffer();
}
void Mouse::OnLeftReleased() noexcept {
    m_IsLeftPressed = false;
    m_Buffer.emplace(Event::Type::LRelease, *this);
    TrimBuffer();
}
void Mouse::OnRightPressed() noexcept {
    m_IsRightPressed = true;
    m_Buffer.emplace(Event::Type::RPress, *this);
    TrimBuffer();
}
void Mouse::OnRightReleased() noexcept {
    m_IsRightPressed = false;
    m_Buffer.emplace(Event::Type::RRelease, *this);
    TrimBuffer();
}

void Mouse::OnWheelUp() noexcept {
    m_Buffer.emplace(Event::Type::WheelUp, *this);
    TrimBuffer();
}
void Mouse::OnWheelDown() noexcept {
    m_Buffer.emplace(Event::Type::WheelDown, *this);
    TrimBuffer();
}
#if !defined(WHEEL_DELTA)
#   define WHEEL_DELTA 120
#endif
void Mouse::OnWheelDelta(int delta) noexcept {
    m_WheelDelta += delta;
    while (m_WheelDelta >= WHEEL_DELTA) {
        m_WheelDelta -= WHEEL_DELTA;
        OnWheelUp();
    }
    while (m_WheelDelta <= -WHEEL_DELTA) {
        m_WheelDelta += WHEEL_DELTA;
        OnWheelDown();
        }
}

void Mouse::TrimBuffer() noexcept {
    while (m_Buffer.size() > c_BufferSize)
        m_Buffer.pop();
}

} //namespace Saba
