#pragma once
#include "World/ECS/Registry.hpp"
#include <tuple>
#include <limits>

namespace Nuim::World {

    template<typename... Cs>
    class View
    {
    public:
        explicit View(Registry& r) : m_r(r) {}

        template<typename Fn>
        void Each(Fn&& fn)
        {
            using FirstT = std::tuple_element_t<0, std::tuple<Cs...>>;

            if (!m_r.template TryGetSet<FirstT>()) return;
            if (!(m_r.template TryGetSet<Cs>() && ...)) return;

            // Choose the smallest dense set among Cs...
            const std::vector<Entity>* baseEnts = nullptr;

            auto tryPick = [&](auto* set)
                {
                    if (!set) return;
                    const auto& ents = set->DenseEntities();
                    if (!baseEnts || ents.size() < baseEnts->size())
                        baseEnts = &ents;
                };

            // Expand for each component type
            (tryPick(m_r.template TryGetSet<Cs>()), ...);

            if (!baseEnts) return;

            for (Entity e : *baseEnts)
            {
                if ((m_r.template Has<Cs>(e) && ...))
                    fn(e, m_r.template Get<Cs>(e)...);
            }
        }

    private:
        Registry& m_r;
    };

}
