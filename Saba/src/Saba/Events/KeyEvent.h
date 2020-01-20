#pragma once

#include "Event.h"

namespace Saba {

	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}
	protected:
		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatedCount)
			: KeyEvent(keycode), m_RepeatedCount(repeatedCount) {}

		inline int GetRepeatedCount() const { return m_RepeatedCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatedCount << " repeated count)";
			return ss.str();
		}

		EVENT_TYPE(KeyPress)
	private:
		int m_RepeatedCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_TYPE(KeyRelease)
	};
	
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_TYPE(KeyType)
	};

}
