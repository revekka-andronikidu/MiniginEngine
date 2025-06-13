#include "ShootingComponent.h"
#include "TimeManager.h"
#include "ObjectFactory.h"
#include "SceneManager.h"

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
   // if (m_TimeSinceLastShot >= m_FireRate && dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("bullet").size() < 2)
   // {
        SpawnBullet();
       // dae::ServiceLocator::GetService<ISoundService>()->PlaySoundEffect("Fighter-Shot.wav");
        //GameMaster::GetInstance().IncrementPlayerShots();
        m_TimeSinceLastShot = 0.0f;
  //  }
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

    SceneManager::GetInstance().GetActiveScene().Add(bullet);
}
