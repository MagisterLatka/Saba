#pragma once

#include "Event.h"

namespace Saba {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float xpos, float ypos)
			: m_XPos(xpos), m_YPos(ypos) {}

		inline float GetXPos() const { return m_XPos; }
		inline float GetYPos() const { return m_YPos; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_XPos << ", " << m_YPos;
			return ss.str();
		}

		EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_TYPE(MouseMove)
	private:
		float m_XPos, m_YPos;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xoffset, float yoffset)
			: m_XOffset(xoffset), m_YOffset(yoffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_TYPE(MouseScroll)
	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int GetButton() const { return m_Button; }

		EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}
	protected:
		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_TYPE(MouseButtonPress)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_TYPE(MouseButtonRelease)
	};
}
