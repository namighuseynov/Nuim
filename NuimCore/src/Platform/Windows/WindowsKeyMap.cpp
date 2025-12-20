#include "NuimPCH.h"
#include "include/Platform/Windows/WindowsKeyMap.hpp"

#ifdef NUIM_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace Nuim {

    KeyCode WindowsKeyMap::ToKeyCode(uint32_t vk)
    {
#ifdef NUIM_PLATFORM_WINDOWS
        // Letters A-Z
        if (vk >= 'A' && vk <= 'Z')
            return (KeyCode)((uint16_t)KeyCode::A + (vk - 'A'));

        // Digits 0-9
        if (vk >= '0' && vk <= '9')
            return (KeyCode)((uint16_t)KeyCode::D0 + (vk - '0'));

        switch (vk)
        {
        case VK_ESCAPE:  return KeyCode::Escape;
        case VK_RETURN:  return KeyCode::Enter;
        case VK_TAB:     return KeyCode::Tab;
        case VK_BACK:    return KeyCode::Backspace;
        case VK_SPACE:   return KeyCode::Space;

        case VK_LEFT:    return KeyCode::Left;
        case VK_RIGHT:   return KeyCode::Right;
        case VK_UP:      return KeyCode::Up;
        case VK_DOWN:    return KeyCode::Down;

        case VK_LSHIFT:  return KeyCode::LShift;
        case VK_RSHIFT:  return KeyCode::RShift;
        case VK_LCONTROL:return KeyCode::LCtrl;
        case VK_RCONTROL:return KeyCode::RCtrl;
        case VK_LMENU:   return KeyCode::LAlt;
        case VK_RMENU:   return KeyCode::RAlt;

        case VK_F1:  return KeyCode::F1;
        case VK_F2:  return KeyCode::F2;
        case VK_F3:  return KeyCode::F3;
        case VK_F4:  return KeyCode::F4;
        case VK_F5:  return KeyCode::F5;
        case VK_F6:  return KeyCode::F6;
        case VK_F7:  return KeyCode::F7;
        case VK_F8:  return KeyCode::F8;
        case VK_F9:  return KeyCode::F9;
        case VK_F10: return KeyCode::F10;
        case VK_F11: return KeyCode::F11;
        case VK_F12: return KeyCode::F12;
        }
#endif
        return KeyCode::Unknown;
    }

}
