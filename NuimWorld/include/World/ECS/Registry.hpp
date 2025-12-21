#pragma once
#include "World/ECS/EntityManager.hpp"
#include "World/ECS/SparseSet.hpp"
#include <memory>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <cassert>

namespace Nuim::World {

    class Registry
    {
        struct IPool
        {
            virtual ~IPool() = default;
            virtual void Remove(Entity e) = 0;
        };

        template<typename T>
        struct Pool final : IPool
        {
            SparseSet<T> set;

            void Remove(Entity e) override { set.Remove(e); }
        };

    public:
        Registry() = default;

        Entity CreateEntity()
        {
            return m_entities.Create();
        }

        void DestroyEntity(Entity e)
        {
            if (!m_entities.IsAlive(e)) return;

            // remove all components from all pools
            for (auto& p : m_pools)
                if (p) p->Remove(e);

            m_entities.Destroy(e);
        }

        bool IsAlive(Entity e) const { return m_entities.IsAlive(e); }

        template<typename T>
        bool Has(Entity e) const
        {
            const auto* p = GetPool<T>();
            return p ? p->set.Has(e) : false;
        }

        template<typename T>
        T& Get(Entity e)
        {
            auto* p = GetPool<T>();
            if (!p)
            {
#if defined(_DEBUG) || defined(DEBUG)
                assert(false && "Registry::Get<const>: pool does not exist for this component type");
#endif
                throw std::runtime_error("Registry::Get<const>: pool does not exist for this component type");
            }
            return p->set.Get(e);
        }

        template<typename T>
        const T& Get(Entity e) const
        {
            const auto* p = GetPool<T>();
            if (!p)
            {
#if defined(_DEBUG) || defined(DEBUG)
                assert(false && "Registry::Get<const>: pool does not exist for this component type");
#endif
                throw std::runtime_error("Registry::Get<const>: pool does not exist for this component type");
            }
            return p->set.Get(e);
        }

        template<typename T, typename... Args>
        T& Emplace(Entity e, Args&&... args)
        {
            auto* p = EnsurePool<T>();
            return p->set.Emplace(e, std::forward<Args>(args)...);
        }

        template<typename T>
        void Remove(Entity e)
        {
            auto* p = GetPool<T>();
            if (p) p->set.Remove(e);
        }

        template<typename T>
        SparseSet<T>* TryGetSet()
        {
            auto* p = GetPool<T>();
            return p ? &p->set : nullptr;
        }

        template<typename T>
        const SparseSet<T>* TryGetSet() const
        {
            const auto* p = GetPool<T>();
            return p ? &p->set : nullptr;
        }

    private:
        template<typename T>
        static size_t TypeId()
        {
            static const size_t id = s_typeCounter++;
            return id;
        }

        template<typename T>
        Pool<T>* EnsurePool()
        {
            const size_t id = TypeId<T>();
            if (id >= m_pools.size())
                m_pools.resize(id + 1);

            if (!m_pools[id])
                m_pools[id] = std::make_unique<Pool<T>>();

            return static_cast<Pool<T>*>(m_pools[id].get());
        }

        template<typename T>
        Pool<T>* GetPool()
        {
            const size_t id = TypeId<T>();
            if (id >= m_pools.size()) return nullptr;
            return static_cast<Pool<T>*>(m_pools[id].get());
        }

        template<typename T>
        const Pool<T>* GetPool() const
        {
            const size_t id = TypeId<T>();
            if (id >= m_pools.size()) return nullptr;
            return static_cast<const Pool<T>*>(m_pools[id].get());
        }

    private:
        EntityManager m_entities;

        std::vector<std::unique_ptr<IPool>> m_pools;
        inline static size_t s_typeCounter = 0;
    };

}
