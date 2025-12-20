#pragma once
#include "Core/Base.hpp"
#include "Core/Events/Event.hpp"
#include "Core/InputCodes.hpp"

namespace Nuim {

    class KeyPressEvent : public Event
    {
    public:
        explicit KeyPressEvent(KeyCode code) : m_code(code) {}

        std::string GetName() const override { return "KeyPress"; }
        EventType GetType() const override { return EventType::KeyPressEvent; }

        KeyCode GetKeyCode() const { return m_code; }

    private:
        KeyCode m_code = KeyCode::Unknown;
    };

    class KeyReleaseEvent : public Event
    {
    public:
        explicit KeyReleaseEvent(KeyCode code) : m_code(code) {}

        std::string GetName() const override { return "KeyRelease"; }
        EventType GetType() const override { return EventType::KeyReleaseEvent; }

        KeyCode GetKeyCode() const { return m_code; }

    private:
        KeyCode m_code = KeyCode::Unknown;
    };

}
