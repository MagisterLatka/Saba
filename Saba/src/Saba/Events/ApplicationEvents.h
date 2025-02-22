#pragma once

#include "Saba/Events/Event.h"
#include "Saba/Core/Timestep.h"

namespace Saba {

class SB_CORE WindowResizeEvent : public Event {
public:
    WindowResizeEvent(uint32_t xsize, uint32_t ysize) noexcept
        : m_XSize(xsize), m_YSize(ysize) {}

    uint32_t GetXSize() const noexcept { return m_XSize; }
    uint32_t GetYSize() const noexcept { return m_YSize; }

    std::string ToString() const noexcept override {
        std::ostringstream oss;
        oss << "WindowResizeEvent: " << m_XSize << ", " << m_YSize;
        return oss.str();
    }

    EVENT_CATEGORY(EventCategoryApplication)
    EVENT_TYPE(WindowResized)
private:
    uint32_t m_XSize, m_YSize;
};

class SB_CORE WindowCloseEvent : public Event {
public:
    WindowCloseEvent() = default;

    std::string ToString() const noexcept override { return "WindowCloseEvent"; }

    EVENT_CATEGORY(EventCategoryApplication)
    EVENT_TYPE(WindowClosed)
};

class SB_CORE WindowFocusEvent : public Event {
public:
    WindowFocusEvent() = default;

    std::string ToString() const noexcept override { return "WindowFocusEvent"; }

    EVENT_CATEGORY(EventCategoryApplication)
    EVENT_TYPE(WindowFocused)
};

class SB_CORE WindowLostFocusEvent : public Event {
public:
    WindowLostFocusEvent() = default;

    std::string ToString() const noexcept override { return "WindowLostFocusEvent"; }

    EVENT_CATEGORY(EventCategoryApplication)
    EVENT_TYPE(WindowLostFocus)
};

class SB_CORE WindowMovedEvent : public Event {
public:
    WindowMovedEvent(int xpos, int ypos) noexcept : m_XPos(xpos), m_YPos(ypos) {}

    int GetXPos() const noexcept { return m_XPos; }
    int GetYPos() const noexcept { return m_YPos; }

    std::string ToString() const noexcept override {
        std::ostringstream oss;
        oss << "WindowMoveEvent: " << m_XPos << ", " << m_YPos;
        return oss.str();
    }

    EVENT_CATEGORY(EventCategoryApplication)
    EVENT_TYPE(WindowMoved)
private:
    int m_XPos, m_YPos;
};

class SB_CORE AppTickEvent : public Event {
public:
    AppTickEvent(Timestep ts) noexcept : m_Timestep(ts) {}

    Timestep GetTimestep() const { return m_Timestep; }

    std::string ToString() const noexcept override {
        std::ostringstream oss;
        oss << "AppTickEvent: " << static_cast<double>(m_Timestep);
        return oss.str();
    }

    EVENT_CATEGORY(EventCategoryApplication)
    EVENT_TYPE(AppTick)
private:
    Timestep m_Timestep;
};

class SB_CORE AppUpdateEvent : public Event {
public:
    AppUpdateEvent() = default;

    std::string ToString() const noexcept override { return "AppUpdateEvent"; }

    EVENT_CATEGORY(EventCategoryApplication)
    EVENT_TYPE(AppUpdate)
};

class SB_CORE AppRenderEvent : public Event {
public:
    AppRenderEvent() = default;

    std::string ToString() const noexcept override { return "AppRenderEvent"; }

    EVENT_CATEGORY(EventCategoryApplication)
    EVENT_TYPE(AppRender)
};

} // namespace Saba
