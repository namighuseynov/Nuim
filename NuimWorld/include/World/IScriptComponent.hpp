#pragma once
#include "Core/UUID.hpp"
#include "Core/Events/Event.hpp"

namespace Nuim::World {

    class GameObject;

    class IScriptComponent
    {
    public: 
        virtual ~IScriptComponent() = default;

        void SetOwner(GameObject* owner) { m_owner = owner; }
        GameObject* GetOwner() const { return m_owner; }

        bool IsAttached() const { return m_attached; }

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnEvent(Nuim::Event& e) {}
        virtual void OnImGuiRender() {}

    protected:
        GameObject* m_owner = nullptr;
        bool m_attached = false;

        friend class GameObject;
    };

}
