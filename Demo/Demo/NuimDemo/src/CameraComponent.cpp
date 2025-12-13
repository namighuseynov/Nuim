#include "NuimDemoPCH.h"
#include "CameraComponent.hpp"

namespace NuimDemo {
	void CameraComponent::OnCreate()
	{
		m_owner->transform.SetPosition({ 0.0f, 0.0f, -5.0f });
		ApplyRotation();
		UpdateRendererMatrices(); 
	}

	void CameraComponent::OnResize(uint32_t w, uint32_t h)
	{
		if (w == 0 || h == 0) return;
		m_aspect = float(w) / float(h);
	}

    void CameraComponent::Update(float dt)
    {
        const float speed = 4.0f;

        float forward = 0.0f;
        float right = 0.0f;

        if (Input::IsKeyDown('W')) forward += 1.0f;
        if (Input::IsKeyDown('S')) forward -= 1.0f;
        if (Input::IsKeyDown('D')) right += 1.0f;
        if (Input::IsKeyDown('A')) right -= 1.0f;

        m_owner->transform.TranslateLocal(
            right * speed * dt,
            0.0f,
            forward * speed * dt
        );

        int dx, dy;
        Input::GetMouseDelta(dx, dy);

        if (Input::IsMouseButtonDown(MouseButton::Right))
        {
            const float sens = 6.0f;

            m_yaw += DirectX::XMConvertToRadians(dx * sens * dt);
            m_pitch += DirectX::XMConvertToRadians(-dy * sens * dt);

            const float limit = DirectX::XMConvertToRadians(89.0f);
            if (m_pitch > limit) m_pitch = limit;
            if (m_pitch < -limit) m_pitch = -limit;

            ApplyRotation();
        }

        UpdateRendererMatrices();
    }

    void CameraComponent::ApplyRotation()
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationRollPitchYaw(m_pitch, m_yaw, 0.0f);
        DirectX::XMFLOAT4 rot;
        DirectX::XMStoreFloat4(&rot, q);
        m_owner->transform.SetRotation(rot);
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