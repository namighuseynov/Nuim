#pragma once
#include "Core/Base.hpp"
#include "RenderTypes.hpp"
#include <memory>

namespace Nuim::Render {

    class Window;

    class IRenderContext {
    public:
        virtual ~IRenderContext() = default;

        virtual Backend GetBackend() const = 0;

        virtual void* GetNativeDevice() const = 0;   
        virtual void* GetNativeContext() const = 0;  
    };
}
