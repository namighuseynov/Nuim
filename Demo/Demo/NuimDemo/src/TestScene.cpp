#include "NuimDemoPCH.h"
#include "TestScene.hpp"

#include "FlyCameraController.hpp"
#include "MeshRenderer.hpp"
#include "Texture2D.hpp"


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

        struct VertexPosUV
        {
            DirectX::XMFLOAT3 pos;
            DirectX::XMFLOAT2 uv;
        };

        static const VertexPosUV kQuadVerts[4] =
        {
            { { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } }, // left-bottom
            { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } }, // left-top
            { {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } }, // right-top
            { {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } }, // right-bottom
        };

        static const uint32_t kQuadIdx[6] = { 0, 1, 2, 0, 2, 3 };

        static const D3D11_INPUT_ELEMENT_DESC kLayoutPosUV[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        auto quadMesh = rm.CreateMesh(
            "Quad",
            engine.GetRenderer()->GetDevice(),
            kQuadVerts,
            sizeof(VertexPosUV),
            4,
            kQuadIdx,
            6
        );

        auto textureMat = rm.CreateMaterial(
            "Quad material",
            engine.GetRenderer()->GetDevice(),
            L"Shaders/TexturedVS.hlsl",
            L"Shaders/TexturedPS.hlsl",
            kLayoutPosUV,
            (UINT)_countof(kLayoutPosUV)
        );

        Nuim::Texture2D tex;
        tex.CreateCheckerboard(engine.GetRenderer()->GetDevice(), 512, 512, 32);

        textureMat->SetTexture(tex.SRV(), tex.Sampler());

        auto& texturedObj = scene.CreateObject();
        texturedObj.SetName("Quad");

        texturedObj.transform.SetPosition({ 0, 0, 0 });
        texturedObj.AddComponent<MeshRenderer>(quadMesh, textureMat);


	}
}