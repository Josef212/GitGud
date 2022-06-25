#pragma once

#include "Event.h"

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
		KeyPressEvent(int keyCode, bool isRepeat = false) : KeyEvent(keyCode), _isRepeat(isRepeat) {}

		inline int IsRepeat() const { return _isRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << _keyCode << " (repeat = " << _isRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPress)

	private:
		bool _isRepeat;
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

	class GG_API KeyTypeEvent : public KeyEvent
	{
	public:
		KeyTypeEvent(int keyCode) : KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypeEvent: " << _keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyType)
	};
}
