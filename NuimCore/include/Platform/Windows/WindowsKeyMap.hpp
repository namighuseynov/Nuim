#pragma once
#include "Core/Base.hpp"
#include "Core/InputCodes.hpp"
#include <cstdint>

namespace Nuim {

    struct WindowsKeyMap
    {
        static KeyCode ToKeyCode(U32 vk);
    };
}
