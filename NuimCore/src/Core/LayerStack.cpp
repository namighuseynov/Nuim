#include "NuimPCH.h"
#include "LayerStack.hpp"

#include <algorithm>

namespace Nuim {

    LayerStack::~LayerStack()
    {
        Clear();
    }

    void LayerStack::PushLayer(std::unique_ptr<Layer> layer)
    {
        if (!layer) return;

        m_layers.emplace(m_layers.begin() + m_insertIndex, std::move(layer));
        m_insertIndex++;
    }

    void LayerStack::PushOverlay(std::unique_ptr<Layer> overlay)
    {
        if (!overlay) return;
        m_layers.emplace_back(std::move(overlay));
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        if (!layer) return;

        auto it = std::find_if(
            m_layers.begin(),
            m_layers.begin() + (ptrdiff_t)m_insertIndex,
            [&](const std::unique_ptr<Layer>& p) { return p.get() == layer; }
        );

        if (it != m_layers.begin() + (ptrdiff_t)m_insertIndex)
        {
            (*it)->OnDetach();
            m_layers.erase(it);
            m_insertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        if (!overlay) return;

        auto it = std::find_if(
            m_layers.begin() + (ptrdiff_t)m_insertIndex,
            m_layers.end(),
            [&](const std::unique_ptr<Layer>& p) { return p.get() == overlay; }
        );

        if (it != m_layers.end())
        {
            (*it)->OnDetach();
            m_layers.erase(it);
        }
    }

    void LayerStack::Clear()
    {
        for (auto& l : m_layers)
            if (l) l->OnDetach();

        m_layers.clear();
        m_insertIndex = 0;
    }

}
