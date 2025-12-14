#pragma once
#include <DirectXMath.h>
#include "GameObject.hpp"
#include "Component.hpp"
#include "Input.hpp"

namespace Nuim {
	class FlyCameraController :
		public Component
	{
    public:
		FlyCameraController(float moveSpeed = 4.0f, float mouseSens = 6.0f)
			: m_moveSpeed(moveSpeed), m_mouseSens(mouseSens) {}

        void Update(float dt) override
        {
            if (!m_owner) return;

            // --- movement WASD ---
            float forward = 0.0f;
            float right = 0.0f;

            if (Input::IsKeyDown('W')) forward += 1.0f;
            if (Input::IsKeyDown('S')) forward -= 1.0f;
            if (Input::IsKeyDown('D')) right += 1.0f;
            if (Input::IsKeyDown('A')) right -= 1.0f;

            m_owner->transform.TranslateLocal(
                right * m_moveSpeed * dt,
                0.0f,
                forward * m_moveSpeed * dt
            );

            // --- mouse (RMB) yaw/pitch ---
            int dx, dy;
            Input::GetMouseDelta(dx, dy);

            if (Input::IsMouseButtonDown(MouseButton::Right))
            {
                m_yaw += DirectX::XMConvertToRadians(dx * m_mouseSens * dt);
                m_pitch -= DirectX::XMConvertToRadians(-dy * m_mouseSens * dt);

                const float limit = DirectX::XMConvertToRadians(89.0f);
                if (m_pitch > limit) m_pitch = limit;
                if (m_pitch < -limit) m_pitch = -limit;

                ApplyRotation();
            }
        }

	private:
		void ApplyRotation()
		{
			using namespace DirectX;

			XMVECTOR q = XMQuaternionRotationRollPitchYaw(m_pitch, m_yaw, 0.0f);
			XMFLOAT4 rot;
			XMStoreFloat4(&rot, q);

			m_owner->transform.SetRotation(rot);
		}

	private:
		float m_moveSpeed = 4.0f;
		float m_mouseSens = 6.0f;

		float m_yaw = 0.0f;
		float m_pitch = 0.0f;
	};

}