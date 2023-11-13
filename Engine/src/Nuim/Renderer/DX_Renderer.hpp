#pragma once
#include "Renderer.hpp"

namespace Nuim {
    class DX_Renderer :
        public Renderer {
    public:
        DX_Renderer();
    public:
        void Begin();
        void End();
    };
}