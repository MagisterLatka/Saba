#pragma once

#include "Saba/Events/Event.h"

namespace Saba {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int xsize, int ysize)
			: m_XSize(xsize), m_YSize(ysize) {}

		int GetXSize() const { return m_XSize; }
		int GetYSize() const { return m_YSize; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_XSize << ", " << m_YSize;
			return ss.str();
		}

		EVENT_CATEGORY(EventCategoryWindow)
		EVENT_TYPE(WindowResize)
	private:
		int m_XSize, m_YSize;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		std::string ToString() const override
		{
			return "WindowCloseEvent";
		}

		EVENT_CATEGORY(EventCategoryWindow)
		EVENT_TYPE(WindowClose)
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {}

		std::string ToString() const override
		{
			return "WindowFocusEvent";
		}

		EVENT_CATEGORY(EventCategoryWindow)
		EVENT_TYPE(WindowFocus)
	};

	class WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() {}

		std::string ToString() const override
		{
			return "WindowLostFocusEvent";
		}

		EVENT_CATEGORY(EventCategoryWindow)
		EVENT_TYPE(WindowLostFocus)
	};
}
