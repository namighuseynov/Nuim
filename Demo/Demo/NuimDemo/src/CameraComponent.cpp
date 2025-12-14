#include "NuimDemoPCH.h"
#include "CameraComponent.hpp"

namespace NuimDemo {
	void CameraComponent::OnCreate()
	{
		m_owner->transform.SetPosition({ 0.0f, 0.0f, -5.0f });
		UpdateRendererMatrices(); 
	}

	void CameraComponent::OnResize(uint32_t w, uint32_t h)
	{
		if (w == 0 || h == 0) return;
		m_aspect = float(w) / float(h);
	}

    void CameraComponent::Update(float dt)
    {
        UpdateRendererMatrices();
    }

    void CameraComponent::UpdateRendererMatrices()
    {
        if (!m_renderer) return;

        using namespace DirectX;

        const XMFLOAT3& posF = m_owner->transform.GetPosition();
        const XMFLOAT4& rotF = m_owner->transform.GetRotation();

        XMVECTOR pos = XMLoadFloat3(&posF);
        XMVECTOR q = XMLoadFloat4(&rotF);

        XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), q);
        XMVECTOR up = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), q);

        XMMATRIX view = XMMatrixLookToLH(pos, forward, up);
        XMMATRIX proj = XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ);

        m_renderer->SetCamera(view, proj);
    }
}