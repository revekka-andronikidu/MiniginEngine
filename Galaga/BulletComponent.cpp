#include "BulletComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include <cmath>

using namespace dae;

BulletComponent::BulletComponent(dae::GameObject* gameObject, float speed, float lifetime, const glm::vec2& direction)
    : BaseComponent(gameObject), m_Direction(direction), m_Speed(speed), m_Lifetime(lifetime)
{
    //UpdateRotation();
}

void BulletComponent::Update()
{
    const float deltaTime = TimeManager::GetInstance().GetDeltaTime();

    //if (!m_TransformComponent) {
       // m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();
       // UpdateRotation();
   // }
   // else {

        const glm::vec2 normalizedDirection = normalize(m_Direction);
        auto locPos = GetOwner()->GetTransform().GetLocalPosition();
        auto newPos = glm::vec3{ normalizedDirection.x * m_Speed * deltaTime, normalizedDirection.y * m_Speed * deltaTime, 0 };
       GetOwner()->GetTransform().SetPosition(locPos + newPos);
  //  }

    m_Lifetime -= deltaTime;
    if (m_Lifetime <= 0.0f)
    {
        GetOwner()->Destroy()   ;
    }
}

void BulletComponent::SetDirection(const glm::vec2& direction)
{
    m_Direction = direction;
    UpdateRotation();
}

void BulletComponent::SetSpeed(float speed)
{
    m_Speed = speed;
}

void BulletComponent::UpdateRotation() const
{
    // Calculate the angle in radians
    float angleRad = std::atan2(m_Direction.y, m_Direction.x);

    // Create a quaternion representing rotation around Z (2D rotation)
    glm::quat rotation = glm::angleAxis(angleRad, glm::vec3(0.0f, 0.0f, 1.0f));

    // Calculate the angle in degrees
   // const float angle = std::atan2(m_Direction.y, m_Direction.x) * (180.0f / glm::pi<float>());

    GetOwner()->GetTransform().SetRotation(rotation);
}