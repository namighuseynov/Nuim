#pragma once
#include "World/ECS/Entity.hpp"
#include <vector>

namespace Nuim::World {

    class EntityManager
    {
    public:
        EntityManager()
        {
            m_generation.push_back(0);
        }

        Entity Create()
        {
            Nuim::U32 id;
            if (!m_free.empty())
            {
                id = m_free.back();
                m_free.pop_back();
            }
            else
            {
                id = (Nuim::U32)m_generation.size();
                m_generation.push_back(0);
            }

            return Entity{ id, m_generation[id] };
        }

        void Destroy(Entity e)
        {
            if (!IsAlive(e)) return;

            // invalidate all old handles to this id
            ++m_generation[e.id];
            m_free.push_back(e.id);
        }

        bool IsAlive(Entity e) const
        {
            return (e.id != 0) &&
                (e.id < m_generation.size()) &&
                (m_generation[e.id] == e.gen);
        }

        void Clear()
        {
            m_generation.clear();
            m_generation.push_back(0); // keep 0 reserved for NullEntity
            m_free.clear();
        }

    private:
        std::vector<Nuim::U32> m_generation;
        std::vector<Nuim::U32> m_free;
    };

}
