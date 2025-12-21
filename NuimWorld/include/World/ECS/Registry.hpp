#pragma once
#include "World/ECS/EntityManager.hpp"
#include "World/ECS/SparseSet.hpp"

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <stdexcept>
#include <cassert>
#include <utility>

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

        void Clear()
        {
            m_pools.clear();
            m_entities.Clear();
        }

        void DestroyEntity(Entity e)
        {
            if (!m_entities.IsAlive(e)) return;

            // remove all components from all pools
            for (auto& kv : m_pools)
                if (kv.second) kv.second->Remove(e);

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
                assert(false && "Registry::Get: pool does not exist for this component type");
#endif
                throw std::runtime_error("Registry::Get: pool does not exist for this component type");
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
                assert(false && "Registry::Get const: pool does not exist for this component type");
#endif
                throw std::runtime_error("Registry::Get const: pool does not exist for this component type");
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
        Pool<T>* EnsurePool()
        {
            const std::type_index key(typeid(T));
            auto it = m_pools.find(key);
            if (it == m_pools.end())
            {
                auto up = std::make_unique<Pool<T>>();
                Pool<T>* raw = up.get();
                m_pools.emplace(key, std::move(up));
                return raw;
            }
            return static_cast<Pool<T>*>(it->second.get());
        }

        template<typename T>
        Pool<T>* GetPool()
        {
            const std::type_index key(typeid(T));
            auto it = m_pools.find(key);
            if (it == m_pools.end()) return nullptr;
            return static_cast<Pool<T>*>(it->second.get());
        }

        template<typename T>
        const Pool<T>* GetPool() const
        {
            const std::type_index key(typeid(T));
            auto it = m_pools.find(key);
            if (it == m_pools.end()) return nullptr;
            return static_cast<const Pool<T>*>(it->second.get());
        }

        

    private:
        EntityManager m_entities;
        std::unordered_map<std::type_index, std::unique_ptr<IPool>> m_pools;
    };

} 
