#pragma once
#include "Component.hpp"
#include "GameObject.hpp"
#include <DirectXMath.h>

namespace Nuim {

    class RotatorComponent : public Component {
    public:
        RotatorComponent(float speedRad = 1.0f) : m_speed(speedRad) {}

        const char* GetTypeName() const override { return "RotatorComponent"; }

        void Update(float dt) override
        {
            if (!m_owner) return;

            m_owner->transform.RotateAxisAngleLocal({ 0.0f, 1.0f, 0.0f }, m_speed * dt);
        }

        float GetSpeed() const { return m_speed; }
        void SetSpeed(float v) { m_speed = v; }

    private:
        float m_speed = 1.0f;
    };

}
