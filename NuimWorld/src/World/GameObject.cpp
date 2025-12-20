#include "World/GameObject.hpp"

namespace Nuim::World {

    template<typename T, typename... Args>
    T& GameObject::AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of_v<IScriptComponent, T>, "T must derive from ScriptComponent");
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        ptr->SetOwner(this);
        ptr->OnAttach();
        m_scripts.emplace_back(std::move(ptr));
        return *static_cast<T*>(m_scripts.back().get());
    }

    template<typename T>
    T* GameObject::GetScript()
    {
        for (auto& s : m_scripts)
            if (auto* p = dynamic_cast<T*>(s.get()))
                return p;
        return nullptr;
    }

    void GameObject::RemoveAllComponents()
    {
        for (auto& s : m_scripts)
            s->OnDetach();
        m_scripts.clear();
    }

    // internal update from Scene
    void GameObject::UpdateComponents(float dt)
    {
        for (auto& s : m_scripts)
            s->OnUpdate(dt);
    }

    void GameObject::DispatchEventToScripts(Nuim::Event& e)
    {
        for (auto& s : m_scripts)
            s->OnEvent(e);
    }

    void GameObject::ImGuiScripts()
    {
        for (auto& s : m_scripts)
            s->OnImGuiRender();
    }
}