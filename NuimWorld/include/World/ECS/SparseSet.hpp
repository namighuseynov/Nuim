#pragma once
#include "World/ECS/Entity.hpp"
#include <vector>
#include <cassert>
#include <utility>

namespace Nuim::World {

    template<typename T>
    class SparseSet
    {
    public:
        bool Has(Entity e) const
        {
            if (e.id >= m_sparse.size()) return false;
            Nuim::U32 idx = m_sparse[e.id];
            return idx != kInvalid && idx < m_denseEnt.size() && m_denseEnt[idx] == e;
        }

        T& Get(Entity e)
        {
            assert(Has(e));
            return m_denseData[m_sparse[e.id]];
        }

        const T& Get(Entity e) const
        {
            assert(Has(e));
            return m_denseData[m_sparse[e.id]];
        }

        template<typename... Args>
        T& Emplace(Entity e, Args&&... args)
        {
            if (e.id >= m_sparse.size()) m_sparse.resize(e.id + 1, kInvalid);
            assert(!Has(e) && "Component already exists on entity");

            Nuim::U32 idx = (Nuim::U32)m_denseEnt.size();
            m_denseEnt.push_back(e);
            m_denseData.emplace_back(std::forward<Args>(args)...);
            m_sparse[e.id] = idx;
            return m_denseData.back();
        }

        void Remove(Entity e)
        {
            if (!Has(e)) return;

            Nuim::U32 idx = m_sparse[e.id];
            Nuim::U32 last = (Nuim::U32)m_denseEnt.size() - 1;

            if (idx != last)
            {
                m_denseEnt[idx] = m_denseEnt[last];
                m_denseData[idx] = std::move(m_denseData[last]);
                m_sparse[m_denseEnt[idx].id] = idx;
            }

            m_denseEnt.pop_back();
            m_denseData.pop_back();
            m_sparse[e.id] = kInvalid;
        }

        const std::vector<Entity>& DenseEntities() const { return m_denseEnt; }
        std::vector<T>& DenseData() { return m_denseData; }
        const std::vector<T>& DenseData() const { return m_denseData; }

    private:
        static constexpr Nuim::U32 kInvalid = 0xFFFFFFFF;

        std::vector<Nuim::U32> m_sparse; 
        std::vector<Entity>    m_denseEnt;  
        std::vector<T>         m_denseData;
    };

} 
