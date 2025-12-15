#include "NuimDemoPCH.h"
#include "TestScene.hpp"

#include "FlyCameraController.hpp"
#include "MeshRenderer.hpp"


namespace Nuim {
	void TestScene::OnLoad(Engine& engine)
	{
        struct VertexColored {
            DirectX::XMFLOAT3 position;
            DirectX::XMFLOAT4 color;
        };

        VertexColored vertices[] = {
        { { -0.5f, -0.5f, -0.5f }, {1.0f, 0.0f, 0.0f, 1.0f} },
        { { -0.5f,  0.5f, -0.5f }, {0.0f, 1.0f, 0.0f, 1.0f} },
        { {  0.5f,  0.5f, -0.5f }, {0.0f, 0.0f, 1.0f, 1.0f} },
        { {  0.5f, -0.5f, -0.5f }, {1.0f, 1.0f, 0.0f, 1.0f} },
        { { -0.5f, -0.5f,  0.5f }, {1.0f, 0.0f, 1.0f, 1.0f} },
        { { -0.5f,  0.5f,  0.5f }, {0.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.5f,  0.5f,  0.5f }, {1.0f, 1.0f, 1.0f, 1.0f} },
        { {  0.5f, -0.5f,  0.5f }, {0.0f, 0.0f, 0.0f, 1.0f} },
        };

        UINT indices[] = {
        0,1,2,  0,2,3,
        4,6,5,  4,7,6,
        4,5,1,  4,1,0,
        3,2,6,  3,6,7,
        1,5,6,  1,6,2,
        4,0,3,  4,3,7
        };

        D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
          0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
          0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        auto& rm = engine.GetResources();

        auto mesh = rm.CreateMesh(
            "Cube",
            engine.GetRenderer()->GetDevice(),
            vertices, sizeof(VertexColored), _countof(vertices),
            indices, _countof(indices)
        );

        auto mat = rm.CreateMaterial(
            "VertexColor",
            engine.GetRenderer()->GetDevice(),
            L"Shaders/VertexShader.hlsl",
            L"Shaders/PixelShader.hlsl",
            layoutDesc, _countof(layoutDesc)
        );
        
        Scene& scene = engine.GetScene();
        const EngineConfig& cfg = engine.GetConfig();

        Nuim::GameObject& cameraObject = scene.CreateObject();
        CameraComponent* cameraComponent = cameraObject.AddComponent<CameraComponent>(engine.GetRenderer(), float(cfg.width) / float(cfg.height));
        
        scene.SetMainCamera(cameraComponent);
        cameraObject.AddComponent<FlyCameraController>(4.0f, 6.0f);

        Nuim::GameObject& cube = scene.CreateObject();
        cube.transform.SetPosition(DirectX::XMFLOAT3(0, 0, 0));
        cube.AddComponent<MeshRenderer>(mesh, mat);

        Nuim::GameObject& cube2 = scene.CreateObject();
        cube2.SetName("Cube2");
        cube2.SetParent(&cube, true);
        cube2.transform.SetPosition(DirectX::XMFLOAT3(0, 0, 2));
        cube2.AddComponent<MeshRenderer>(mesh, mat);
	}
}