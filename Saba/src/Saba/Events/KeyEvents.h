#pragma once

#include "Saba/Events/Event.h"
#include "Saba/Core/KeyCodes.h"

namespace Saba {

class SB_CORE KeyEvent : public Event {
public:
    KeyCode GetKeyCode() const noexcept { return m_KeyCode; }

    EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
protected:
    KeyEvent(KeyCode keycode) noexcept : m_KeyCode(keycode) {}
protected:
    KeyCode m_KeyCode;
};

class SB_CORE KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(KeyCode keycode, int repeatedCount) noexcept
        : KeyEvent(keycode), m_RepeatedCount(repeatedCount) {}

    int GetRepeatedCount() const noexcept { return m_RepeatedCount; }

    std::string ToString() const noexcept override {
        std::ostringstream oss;
        oss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatedCount
            << " repeated count)";
        return oss.str();
    }

    EVENT_TYPE(KeyPressed)
private:
    int m_RepeatedCount;
};

class SB_CORE KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(KeyCode keycode) noexcept : KeyEvent(keycode) {}

    std::string ToString() const noexcept override {
        std::ostringstream oss;
        oss << "KeyReleasedEvent: " << m_KeyCode;
        return oss.str();
    }
    EVENT_TYPE(KeyReleased)
};

class SB_CORE KeyTypedEvent : public Event {
public:
    KeyTypedEvent(uint8_t code) noexcept : m_Code(code) {}

    std::string ToString() const noexcept override {
        std::ostringstream oss;
        oss << "KeyTypedEvent: " << m_Code;
        return oss.str();
    }

    EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
    EVENT_TYPE(KeyTyped)
private:
    uint8_t m_Code;
};

} // namespace Saba
