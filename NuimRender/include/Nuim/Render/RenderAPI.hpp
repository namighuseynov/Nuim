#pragma once
#include "Core/Base.hpp"

namespace Nuim {

    enum class RenderAPIType : U8 {
        None = 0,
        DX11 = 1
    };

    class RenderAPI {
    public:
        static void Set(RenderAPIType type) { s_type = type; }
        static RenderAPIType Get() { return s_type; }

    private:
        inline static RenderAPIType s_type = RenderAPIType::None;
    };

}