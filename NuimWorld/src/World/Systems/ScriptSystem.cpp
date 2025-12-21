#include "World/Systems/ScriptSystem.hpp"
#include "World/Scripting/ScriptBase.hpp"

namespace Nuim::World {

    void ScriptSystem::DetachIfRunning(Entity e)
    {
        if (!m_r.Has<ScriptComponent>(e)) return;
        auto& sc = m_r.Get<ScriptComponent>(e);

        for (auto& s : sc.scripts)
        {
            if (!s) continue;
            if (s->m_attached)
            {
                s->OnDetach();
                s->_SetAttached(false);
            }
        }
    }

    void ScriptSystem::OnEntityDestroy(Entity e)
    {
        DetachIfRunning(e);
    }

    void ScriptSystem::OnRuntimeStart()
    {
        View<ScriptComponent> v(m_r);
        v.Each([this](Entity e, ScriptComponent& sc)
            {
                for (auto& s : sc.scripts)
                {
                    if (!s) continue;
                    if (!s->m_attached)
                    {
                        s->_Bind(&m_w, e);
                        s->_SetAttached(true);
                        s->OnAttach();
                    }
                }
            });
    }

    void ScriptSystem::OnRuntimeStop()
    {
        View<ScriptComponent> v(m_r);
        v.Each([&](Entity, ScriptComponent& sc)
            {
                for (auto& s : sc.scripts)
                {
                    if (!s) continue;
                    if (s->m_attached)
                    {
                        s->OnDetach();
                        s->_SetAttached(false);
                    }
                }
            });
    }

    void ScriptSystem::Update(float dt)
    {
        View<ScriptComponent> v(m_r);
        v.Each([&](Entity, ScriptComponent& sc)
            {
                for (auto& s : sc.scripts)
                    if (s && s->m_attached)
                        s->OnUpdate(dt);
            });
    }

    void ScriptSystem::DispatchEvent(Nuim::Event& e)
    {
        View<ScriptComponent> v(m_r);
        v.Each([&](Entity, ScriptComponent& sc)
            {
                for (auto& s : sc.scripts)
                    if (s && s->m_attached)
                        s->OnEvent(e);
            });
    }

} 
