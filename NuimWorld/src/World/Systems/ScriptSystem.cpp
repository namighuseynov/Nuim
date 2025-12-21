#include "World/Systems/ScriptSystem.hpp"
#include "World/Scripting/ScriptBase.hpp"
#include "World/Components/ActiveComponent.hpp"

namespace Nuim::World {

    static bool IsEntityActive(const Registry& r, Entity e)
    {
        if (!r.Has<ActiveComponent>(e)) return true;
        return r.Get<ActiveComponent>(e).active;
    }

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

    void ScriptSystem::AttachIfRunning(Entity e)
    {
        if (!m_r.Has<ScriptComponent>(e)) return;
        if (!IsEntityActive(m_r, e)) return;

        auto& sc = m_r.Get<ScriptComponent>(e);
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
    }

    void ScriptSystem::OnEntityDestroy(Entity e)
    {
        DetachIfRunning(e);
    }

    void ScriptSystem::OnEntityActiveChanged(Entity e, bool active)
    {
        if (!m_w.IsRunning()) return;

        if (active) AttachIfRunning(e);
        else        DetachIfRunning(e);
    }

    void ScriptSystem::OnRuntimeStart()
    {
        View<ScriptComponent> v(m_r);
        v.Each([this](Entity e, ScriptComponent&)
            {
                if (!IsEntityActive(m_r, e)) return;
                AttachIfRunning(e);
            });
    }

    void ScriptSystem::OnRuntimeStop()
    {
        View<ScriptComponent> v(m_r);
        v.Each([this](Entity e, ScriptComponent&)
            {
                DetachIfRunning(e);
            });
    }

    void ScriptSystem::Update(float dt)
    {
        View<ScriptComponent> v(m_r);
        v.Each([&](Entity e, ScriptComponent& sc)
            {
                if (!IsEntityActive(m_r, e)) return;

                for (auto& s : sc.scripts)
                    if (s && s->m_attached)
                        s->OnUpdate(dt);
            });
    }

    void ScriptSystem::FixedUpdate(float fixedDt)
    {
        View<ScriptComponent> v(m_r);
        v.Each([&](Entity e, ScriptComponent& sc)
            {
                if (!IsEntityActive(m_r, e)) return;

                for (auto& s : sc.scripts)
                    if (s && s->m_attached)
                        s->OnFixedUpdate(fixedDt);
            });
    }

    void ScriptSystem::DispatchEvent(Nuim::Event& e)
    {
        View<ScriptComponent> v(m_r);
        v.Each([&](Entity ent, ScriptComponent& sc)
            {
                if (!IsEntityActive(m_r, ent)) return;

                for (auto& s : sc.scripts)
                    if (s && s->m_attached)
                        s->OnEvent(e);
            });
    }

}
