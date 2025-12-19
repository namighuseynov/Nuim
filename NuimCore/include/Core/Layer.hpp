#pragma once
#include "Base.hpp"

#include <string>

namespace Nuim {

    class Event;

    class Layer
    {
    public:
        explicit Layer(std::string name = "Layer")
            : m_name(std::move(name)) {
        }

        virtual ~Layer() = default;

        // lifecycle
        virtual void OnAttach() {}
        virtual void OnDetach() {}

        // per-frame
        virtual void OnUpdate(float dt) {}
        virtual void OnImGuiRender() {}

        // events
        virtual void OnEvent(Event& e) {}

        const std::string& GetName() const { return m_name; }

    private:
        std::string m_name;
    };
}
