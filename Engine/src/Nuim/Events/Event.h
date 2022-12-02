#pragma once

#include "Nuim/Core/Core.h"

#include <functional>

namespace Nuim {
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowUnFocus, WindowMoved,
		ApplicationTick, ApplicationUpdate, ApplicationRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		ApplicationEventCategory = BIT(0),
		InputEventCategory = BIT(1),
		KeyboardEventCategory = BIT(2),
		MouseEventCategory = BIT(3),
		MouseButtonEventCategory = BIT(4)
	};


	class Event {
	public:
		virtual ~Event() = default;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		// F will be deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};


	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}


