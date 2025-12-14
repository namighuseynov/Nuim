#pragma once

namespace Nuim {
    class Engine;

    class IScene {
    public:
        virtual ~IScene() = default;
        virtual void OnLoad(Engine& engine) = 0;
        virtual void OnUnload() {}
    };
}