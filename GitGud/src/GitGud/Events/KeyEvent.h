#pragma once

#include "Event.h"

#include <sstream>

namespace GitGud
{
	class GG_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return _keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keyCode) : _keyCode(keyCode) {}

		int _keyCode;
	};

	class GG_API KeyPressEvent : public KeyEvent
	{
	public:
		KeyPressEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), _repeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return _repeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPress)

	private:
		int _repeatCount;
	};

	class GG_API KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keyCode) : KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << _keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyRelease)
	};
}
