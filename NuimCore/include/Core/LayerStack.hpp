#pragma once
#include "Base.hpp"
#include "Layer.hpp"

#include <vector>
#include <memory>

namespace Nuim {

    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(std::unique_ptr<Layer> layer);
        void PushOverlay(std::unique_ptr<Layer> overlay);

        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        void Clear();

        auto begin() { return m_layers.begin(); }
        auto end() { return m_layers.end(); }
        auto begin() const { return m_layers.begin(); }
        auto end()   const { return m_layers.end(); }

        auto rbegin() { return m_layers.rbegin(); }
        auto rend() { return m_layers.rend(); }
        auto rbegin() const { return m_layers.rbegin(); }
        auto rend()   const { return m_layers.rend(); }

    private:
        std::vector<std::unique_ptr<Layer>> m_layers;
        U32 m_insertIndex = 0; 
    };

}
