#pragma once

namespace Saba {

	enum class EventType
	{
		NONE = 0,
		KeyPress, KeyRelease, KeyType,
		MouseMove, MouseScroll, MouseButtonPress, MouseButtonRelease,
		WindowResize, WindowClose, WindowFocus, WindowLostFocus
	};

	enum EventCategory
	{
		NONE = 0,
		EventCategoryWindow			= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

#define EVENT_TYPE(x) static EventType GetStaticType() { return EventType::x; }\
						virtual EventType GetEventType() const override { return GetStaticType(); }\
						virtual const char* GetName() const override { return #x; }
#define EVENT_CATEGORY(x) virtual int GetCategoryFlags() const override { return x; }

	class Event
	{
	public:
		bool p_Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class Dispatcher
	{
	public:
		Dispatcher(Event& e)
			: m_Event(e) {}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.p_Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& stream, Event& e)
	{
		return stream << e.ToString();
	}

}
