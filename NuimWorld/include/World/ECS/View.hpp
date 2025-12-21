#pragma once
#include "World/ECS/Registry.hpp"
#include <tuple>

namespace Nuim::World {

    template<typename... Cs>
    class View
    {
    public:
        explicit View(Registry& r) : m_r(r) {}

        template<typename Fn>
        void Each(Fn&& fn)
        {
            // iterate by the smallest dense set (cheap heuristic: first component)
            auto* base = m_r.template TryGetSet<std::tuple_element_t<0, std::tuple<Cs...>>>();
            if (!base) return;

            const auto& ents = base->DenseEntities();
            for (Entity e : ents)
            {
                if ((m_r.template Has<Cs>(e) && ...))
                    fn(e, m_r.template Get<Cs>(e)...);
            }
        }

    private:
        Registry& m_r;
    };

} // namespace Nuim::World
