#pragma once

#include "Event.h"

namespace GitGud
{
	class GG_API MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y) : _mouseX(x), _mouseY(y) {}

		inline float GetX() { return _mouseX; }
		inline float GetY() { return _mouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << _mouseX << ", " << _mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMove)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float _mouseX, _mouseY;
	};

	class GG_API MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xOffset, float yOffset) : _offsetX(xOffset), _offsetY(yOffset) {}

		inline float GetXOffset() { return _offsetX; }
		inline float GetYOffset() { return _offsetY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << _offsetX << ", " << _offsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScroll)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float _offsetX, _offsetY;
	};

	class GG_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return _button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button) : _button(button) {}

		int _button;
	};

	class GG_API MouseButtonPressEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressEvent: " << _button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPress)
	};

	class GG_API MouseButtonReleaseEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleaseEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleaseEvent: " << _button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonRelease)
	};
}
