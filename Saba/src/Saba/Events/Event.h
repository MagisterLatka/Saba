#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

enum class EventType : uint8_t {
    None = 0,
    WindowResized, WindowClosed, WindowFocused, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased, KeyTyped,
    MouseMoved, MouseScrolled, MouseButtonPressed, MouseButtonReleased, MouseEntered, MouseLeft
};

enum EventCategory : uint8_t {
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3),
    EventCategoryMouseButton = BIT(4)
};

#define EVENT_TYPE(x) static EventType GetStaticType() noexcept { return EventType::x; }\
        EventType GetEventType() const noexcept override { return GetStaticType(); }\
        const char *GetName() const noexcept override { return #x; }
#define EVENT_CATEGORY(x) int GetCategoryFlags() const noexcept override { return x; }

class SB_CORE Event {
public:
    virtual ~Event() = default;

    virtual EventType GetEventType() const noexcept = 0;
    virtual const char *GetName() const noexcept = 0;
    virtual int GetCategoryFlags() const noexcept = 0;
    virtual std::string ToString() const noexcept { return GetName(); }

    bool IsInCategory(EventCategory category) const noexcept {
      return (GetCategoryFlags() & category) != 0;
    }

public:
    bool p_Handled = false;
};

class SB_CORE Dispatcher {
public:
    Dispatcher(Event &e) noexcept : m_Event(e) {}

    template <typename T, typename F>
    requires (std::is_base_of_v<Event, T> && std::is_convertible_v<std::invoke_result_t<F, T&>, bool>)
    bool Dispatch(const F& fn) noexcept {
        if (m_Event.GetEventType() == T::GetStaticType()) {
            m_Event.p_Handled |= fn(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }
private:
    Event &m_Event;
};

inline std::ostream &operator<<(std::ostream &stream, const Event &e) noexcept {
    return stream << e.ToString();
}

} // namespace Saba
