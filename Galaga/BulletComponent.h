#pragma once
#include "BaseComponent.h"
#include <vec2.hpp>

namespace dae
{
    class BulletComponent final : public dae::BaseComponent
    {
    public:
        BulletComponent(dae::GameObject* gameObject, float speed = 150.f, float lifetime = 2.0f, const glm::vec2& direction = glm::vec2(0, -1));
        ~BulletComponent() = default;

        void Update() override;
        void FixedUpdate(const float fixedTimeStep) override {};
        void SetDirection(const glm::vec2& direction);
        void SetSpeed(float speed);

    private:
        glm::vec2 m_Direction;
        float m_Speed;
        float m_Lifetime;

        void UpdateRotation() const;
    };
}
