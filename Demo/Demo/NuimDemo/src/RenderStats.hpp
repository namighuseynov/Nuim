#pragma once
#include <cstdint>

namespace Nuim {

    struct RenderStats {
        uint32_t drawCalls = 0;
        uint32_t triangles = 0;

        void Reset() {
            drawCalls = 0;
            triangles = 0;
        }
    };

}