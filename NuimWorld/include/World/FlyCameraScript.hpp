#pragma once
#include <DirectXMath.h>
#include "World/GameObject.hpp"
#include "World/IScriptComponent.hpp"
#include "Core/Input.hpp"
#include "Core/Events/MouseEvent.hpp"

namespace Nuim::World {

    class FlyCameraScript final : public IScriptComponent
    {
    public:
        FlyCameraScript(float moveSpeed = 4.0f, float mouseSens = 6.0f)
            : m_moveSpeed(moveSpeed), m_mouseSens(mouseSens) {}

        void SetEnabled(bool enabled) { m_enabled = enabled; }
        bool IsEnabled() const { return m_enabled; }

        void OnAttach() override
        {
            m_yaw = 0.0f;
            m_pitch = 0.0f;
            ApplyRotation();
        }

        void OnUpdate(float dt) override
        {
            if (!m_owner || !m_enabled) return;

            float forward = 0.0f;
            float right = 0.0f;
            float up = 0.0f;

            if (Nuim::Input::IsKeyDown('W')) forward += 1.0f;
            if (Nuim::Input::IsKeyDown('S')) forward -= 1.0f;
            if (Nuim::Input::IsKeyDown('D')) right += 1.0f;
            if (Nuim::Input::IsKeyDown('A')) right -= 1.0f;


            if (Nuim::Input::IsKeyDown('E')) up += 1.0f;
            if (Nuim::Input::IsKeyDown('Q')) up -= 1.0f;

            float speed = m_moveSpeed;


            auto& tr = m_owner->GetTransform();

            tr.TranslateLocal(right * speed * dt, 0.0f, forward * speed * dt);


            tr.TranslateWorld(0.0f, up * speed * dt, 0.0f);

            if (Nuim::Input::IsMouseButtonDown(Nuim::MouseButton::NM_RIGHT))
            {
                U32 dx = 0, dy = 0;
                Nuim::Input::GetMouseDelta(dx, dy);

                m_yaw += DirectX::XMConvertToRadians((float)dx * m_mouseSens * dt);
                m_pitch -= DirectX::XMConvertToRadians((float)dy * m_mouseSens * dt);

                const float limit = DirectX::XMConvertToRadians(89.0f);
                if (m_pitch > limit) m_pitch = limit;
                if (m_pitch < -limit) m_pitch = -limit;

                ApplyRotation();
            }
        }

        DirectX::XMMATRIX GetViewMatrix() const
        {
            using namespace DirectX;

            if (!m_owner)
                return XMMatrixIdentity();

            const auto& tr = m_owner->GetTransform();
            const XMFLOAT3 posF = tr.GetLocalPosition();
            const XMFLOAT4 rotF = tr.GetLocalRotation();

            XMVECTOR pos = XMLoadFloat3(&posF);
            XMVECTOR q = XMQuaternionNormalize(XMLoadFloat4(&rotF));

            XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), q);
            XMVECTOR up = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), q);

            return XMMatrixLookToLH(pos, forward, up);
        }

        DirectX::XMMATRIX GetProjMatrix(float aspect) const
        {
            using namespace DirectX;
            aspect = (aspect <= 0.0f) ? 1.0f : aspect;
            return XMMatrixPerspectiveFovLH(
                XMConvertToRadians(m_fovDeg),
                aspect,
                m_nearZ,
                m_farZ
            );
        }

        void SetProjection(float fovDeg, float nearZ, float farZ)
        {
            m_fovDeg = fovDeg;
            m_nearZ = nearZ;
            m_farZ = farZ;
        }

    private:
        void ApplyRotation()
        {
            using namespace DirectX;

            XMVECTOR q = XMQuaternionRotationRollPitchYaw(m_pitch, m_yaw, 0.0f);
            XMFLOAT4 rot;
            XMStoreFloat4(&rot, q);

            m_owner->GetTransform().SetLocalRotation(rot);
        }

    private:
        bool  m_enabled = true;

        float m_moveSpeed = 4.0f;
        float m_mouseSens = 6.0f;

        float m_yaw = 0.0f;
        float m_pitch = 0.0f;

        float m_fovDeg = 60.0f;
        float m_nearZ = 0.1f;
        float m_farZ = 200.0f;
    };

}
