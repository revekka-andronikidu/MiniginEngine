#include "ShootingComponent.h"
#include "TimeManager.h"
#include "ObjectFactory.h"
#include "SceneManager.h"
#include "GalagaGame.h"
#include "GameManager.h"
#include "ServiceLocator.h"

using namespace dae;
ShootingComponent::ShootingComponent(dae::GameObject* gameObject, float fireRate) 
    : m_FireRate{fireRate}
    , BaseComponent(gameObject)
{
}

void ShootingComponent::Update()
{
    m_TimeSinceLastShot += TimeManager::GetInstance().GetDeltaTime();
}

void ShootingComponent::Shoot()
{
    //TODO: Limit bullets in scene
    if (m_TimeSinceLastShot >= m_FireRate)
    {
        SpawnBullet();
        ServiceLocator::GetAudioService().PlayEffect(1, 0.8f, false);
        auto* galaga = dynamic_cast<GalagaGame*>(GameManager::GetInstance().GetActiveGame());
        galaga->IncrementShotsFired();
        m_TimeSinceLastShot = 0.0f;
    }
}

void ShootingComponent::SetFireRate(float rpm)
{
    m_FireRate = TimeManager::GetInstance().GetFPS() / rpm;
}

void ShootingComponent::SpawnBullet() const
{

   const auto playerPosition = GetOwner()->GetTransform().GetWorldPosition();
   auto textureSize = GetOwner()->GetComponent<TextureComponent>()->GetTextureSize() ;
   textureSize = { textureSize.x * GetOwner()->GetTransform().GetScale().x, textureSize.y * GetOwner()->GetTransform().GetScale().y };

   auto spawnPosition = glm::vec3{ playerPosition.x + (textureSize.x / 2), playerPosition.y, playerPosition.z };

    auto bullet = ObjectFactory::GetInstance().CreateBullet(spawnPosition);

    SceneManager::GetInstance().GetActiveScene().Add(std::move(bullet));
}
