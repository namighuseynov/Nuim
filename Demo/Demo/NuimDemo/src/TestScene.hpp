#pragma once
#include "IScene.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Engine.hpp"

namespace Nuim {

    class TestScene : 
        public IScene {
    public:
        void OnLoad(Engine& engine) override;
        void OnUnload() override {}

    private:
        Mesh m_cubeMesh;
        Material m_cubeMat;
    };
}
