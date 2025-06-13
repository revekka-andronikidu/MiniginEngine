#pragma once
#include "BaseComponent.h"


namespace dae
{
    class Bullet;

    class ShootingComponent final : public dae::BaseComponent
    {
    public:
        ShootingComponent(dae::GameObject* gameObject, float fireRate = 0.5f);
        ~ShootingComponent() = default;

        void Update() override;
        void FixedUpdate(const float fixedTimeStep) override {};

        void Shoot();
        void SetFireRate(float rpm);

    private:
        void SpawnBullet() const;
        float m_FireRate{ };
        float m_TimeSinceLastShot{ 0.0f };
    };
}