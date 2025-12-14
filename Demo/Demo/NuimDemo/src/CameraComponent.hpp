#pragma once
#include <DirectXMath.h>
#include "GameObject.hpp"
#include "Component.hpp"
#include "Renderer/Renderer.hpp"
#include "Input.hpp"

namespace Nuim {
	class CameraComponent : 
		public Component
	{
    public:
        CameraComponent(Renderer* renderer, float aspect)
            : m_renderer(renderer), m_aspect(aspect)
        {
            m_owner = nullptr;
        }

        const char* GetTypeName() const override { return "CameraComponent"; }

        void OnCreate() override;

		void OnResize(uint32_t  width, uint32_t  height);

		void LateUpdate(float dt) override;

    private:
        void UpdateRendererMatrices();


    private:
        Renderer* m_renderer = nullptr;

        // Projection
        float m_fovY = DirectX::XMConvertToRadians(45.0f);
        float m_aspect = 16.0f / 9.0f;
        float m_nearZ = 0.1f;
        float m_farZ = 100.0f;
	};
}