#pragma once

#include "Saba/Events/Event.h"
#include "Saba/Core/MouseCodes.h"

namespace Saba {

class SB_CORE MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float xpos, float ypos) noexcept
        : m_XPos(xpos), m_YPos(ypos) {}

    float GetXPos() const noexcept { return m_XPos; }
    float GetYPos() const noexcept { return m_YPos; }

    std::string ToString() const noexcept override {
        std::ostringstream oss;
        oss << "MouseMovedEvent: " << m_XPos << ", " << m_YPos;
        return oss.str();
    }

    EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)
    EVENT_TYPE(MouseMoved)
private:
    float m_XPos, m_YPos;
};

class SB_CORE MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float xoffset, float yoffset) noexcept
        : m_XOffset(xoffset), m_YOffset(yoffset) {}

    float GetXOffset() const noexcept { return m_XOffset; }
    float GetYOffset() const noexcept { return m_YOffset; }

    std::string ToString() const noexcept override {
        std::ostringstream oss;
        oss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
        return oss.str();
    }

  EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)
  EVENT_TYPE(MouseScrolled)
private:
  float m_XOffset, m_YOffset;
};

class SB_CORE MouseButtonEvent : public Event {
public:
    MouseCode GetButton() const noexcept { return m_Button; }

    EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)
protected:
    MouseButtonEvent(MouseCode button) noexcept : m_Button(button) {}
protected:
    MouseCode m_Button;
};

class SB_CORE MouseButtonPressedEvent : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(MouseCode button) noexcept
        : MouseButtonEvent(button) {}

    std::string ToString() const noexcept override {
        std::stringstream oss;
        oss << "MouseButtonPressedEvent: " << m_Button;
        return oss.str();
    }

    EVENT_TYPE(MouseButtonPressed)
};

class SB_CORE MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(MouseCode button) noexcept
        : MouseButtonEvent(button) {}

    std::string ToString() const noexcept override {
        std::stringstream oss;
        oss << "MouseButtonReleasedEvent: " << m_Button;
        return oss.str();
    }

    EVENT_TYPE(MouseButtonReleased)
};

class SB_CORE MouseEnteredEvent : public Event {
public:
    MouseEnteredEvent() = default;

    std::string ToString() const noexcept override { return "MouseEnteredEvent"; }

    EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)
    EVENT_TYPE(MouseEntered)
};

class SB_CORE MouseLeftEvent : public Event {
public:
    MouseLeftEvent() = default;

    std::string ToString() const noexcept override { return "MouseLeftEvent"; }

    EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)
    EVENT_TYPE(MouseLeft)
};
} // namespace Saba
