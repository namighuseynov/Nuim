#pragma once
#include "FlyCameraController.hpp"
#include "MeshRenderer.hpp"
#include "TestScene.hpp"

namespace Nuim {
    class Application {
    public:
        Application(const HINSTANCE& instance) {};
        ~Application() = default;
    public:
        int Run() {
            EngineConfig cfg{ 1280, 800, true };

            Engine engine;
            if (!engine.Init(cfg)) return -1;

            engine.LoadScene(std::make_unique<TestScene>());

            engine.Run();
            return 0;
        } 
    };
}