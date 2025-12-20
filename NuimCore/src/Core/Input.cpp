#include "NuimPCH.h"
#include "Core/Input.hpp"

#include <cstring> // memset

namespace Nuim {

    // ----------------------------
    // Static storage
    // ----------------------------
    bool Input::s_keysDown[Input::s_maxKeys] = {};
    bool Input::s_keysPressed[Input::s_maxKeys] = {};
    bool Input::s_keysReleased[Input::s_maxKeys] = {};

    bool Input::s_mouseButtonDown[3] = {};
    bool Input::s_mouseButtonPressed[3] = {};
    bool Input::s_mouseButtonUp[3] = {};

    U32 Input::s_mouseX = 0;
    U32 Input::s_mouseY = 0;


    I32 Input::s_mouseDeltaX = 0;
    I32 Input::s_mouseDeltaY = 0;

    bool Input::s_initialized = false;

    // ----------------------------
    // Helpers
    // ----------------------------
    static inline U32 ClampMouseIndex(MouseButton b)
    {
        const U32 idx = (U32)(U8)b;
        return (idx < 3) ? idx : 0;
    }

    // ----------------------------
    // Public API
    // ----------------------------
    void Input::Init()
    {
        std::memset(s_keysDown, 0, sizeof(s_keysDown));
        std::memset(s_keysPressed, 0, sizeof(s_keysPressed));
        std::memset(s_keysReleased, 0, sizeof(s_keysReleased));

        std::memset(s_mouseButtonDown, 0, sizeof(s_mouseButtonDown));
        std::memset(s_mouseButtonPressed, 0, sizeof(s_mouseButtonPressed));
        std::memset(s_mouseButtonUp, 0, sizeof(s_mouseButtonUp));

        s_mouseX = s_mouseY = 0;
        s_mouseDeltaX = s_mouseDeltaY = 0;

        s_initialized = true;
    }

    void Input::NewFrame()
    {
        if (!s_initialized)
            return;

        std::memset(s_keysPressed, 0, sizeof(s_keysPressed));
        std::memset(s_keysReleased, 0, sizeof(s_keysReleased));

        std::memset(s_mouseButtonPressed, 0, sizeof(s_mouseButtonPressed));
        std::memset(s_mouseButtonUp, 0, sizeof(s_mouseButtonUp));

        s_mouseDeltaX = 0;
        s_mouseDeltaY = 0;
    }

    void Input::OnKeyDown(KeyCode key)
    {
        if (!s_initialized)
            return;

        const U32 idx = ToIndex(key);
        if (idx >= s_maxKeys)
            return;

        if (!s_keysDown[idx])
            s_keysPressed[idx] = true;

        s_keysDown[idx] = true;
    }

    void Input::OnKeyUp(KeyCode key)
    {
        if (!s_initialized)
            return;

        const U32 idx = ToIndex(key);
        if (idx >= s_maxKeys)
            return;

        if (s_keysDown[idx])
            s_keysReleased[idx] = true;

        s_keysDown[idx] = false;
    }

    bool Input::IsKeyDown(KeyCode key)
    {
        if (!s_initialized)
            return false;

        const U32 idx = ToIndex(key);
        if (idx >= s_maxKeys)
            return false;

        return s_keysDown[idx];
    }

    bool Input::IsKeyPressed(KeyCode key)
    {
        if (!s_initialized)
            return false;

        const U32 idx = ToIndex(key);
        if (idx >= s_maxKeys)
            return false;

        return s_keysPressed[idx];
    }

    bool Input::IsKeyReleased(KeyCode key)
    {
        if (!s_initialized)
            return false;

        const U32 idx = ToIndex(key);
        if (idx >= s_maxKeys)
            return false;

        return s_keysReleased[idx];
    }

    void Input::OnMouseMove(U32 x, U32 y)
    {
        if (!s_initialized)
            return;

        const I32 dx = (I32)x - (I32)s_mouseX;
        const I32 dy = (I32)y - (I32)s_mouseY;

        s_mouseDeltaX += dx;
        s_mouseDeltaY += dy;

        s_mouseX = x;
        s_mouseY = y;
    }

    void Input::OnMouseButtonDown(MouseButton button)
    {
        if (!s_initialized)
            return;

        const U32 idx = ClampMouseIndex(button);

        if (!s_mouseButtonDown[idx])
            s_mouseButtonPressed[idx] = true;

        s_mouseButtonDown[idx] = true;
    }

    void Input::OnMouseButtonUp(MouseButton button)
    {
        if (!s_initialized)
            return;

        const U32 idx = ClampMouseIndex(button);

        if (s_mouseButtonDown[idx])
            s_mouseButtonUp[idx] = true;

        s_mouseButtonDown[idx] = false;
    }

    bool Input::IsMouseButtonDown(MouseButton button)
    {
        if (!s_initialized)
            return false;

        const U32 idx = ClampMouseIndex(button);
        return s_mouseButtonDown[idx];
    }

    bool Input::IsMouseButtonPressed(MouseButton button)
    {
        if (!s_initialized)
            return false;

        const U32 idx = ClampMouseIndex(button);
        return s_mouseButtonPressed[idx];
    }

    bool Input::IsMouseButtonReleased(MouseButton button)
    {
        if (!s_initialized)
            return false;

        const U32 idx = ClampMouseIndex(button);
        return s_mouseButtonUp[idx];
    }

    void Input::GetMousePosition(U32& x, U32& y)
    {
        x = s_mouseX;
        y = s_mouseY;
    }

    void Input::GetMouseDelta(I32& dx, I32& dy)
    {
        dx = s_mouseDeltaX;
        dy = s_mouseDeltaY;
    }

}
