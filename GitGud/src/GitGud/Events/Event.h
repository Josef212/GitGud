#pragma once

#include "ggpch.h"
#include "GitGud/Core/Core.h"

namespace GitGud
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPress, KeyRelease, KeyType,
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= 1 << 0,
		EventCategoryInput			= 1 << 1,
		EventCategoryKeyboard		= 1 << 2,
		EventCategoryMouse			= 1 << 3,
		EventCategoryMouseButton	= 1 << 4
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class GG_API Event
	{
	public:
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : _event(event) {}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (_event.GetEventType() == T::GetStaticType())
			{
				_event.Handled |= func(static_cast<T&>(_event));
				return true;
			}

			return false;
		}

	private:
		Event& _event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}