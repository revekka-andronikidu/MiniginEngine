#include "EnemyAttackComponent.h"
#include "TimeManager.h"
#include "ObjectFactory.h"
#include <iostream>
#include "SpriteComponent.h"

using namespace dae;

void EnemyAttackComponent::Update()
{
	m_ShootTimer += dae::TimeManager::GetInstance().GetDeltaTime();

	if (m_ShootTimer >= m_ShootInterval)
	{
		Shoot();
		m_ShootTimer = 0;
		std::cout << "Shots fired" << std::endl;
	}
}

void EnemyAttackComponent::Shoot()
{
	auto players = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag("player");

	if (players.empty()) return;

	//choose random or closes player?
	auto playerPos = players[0]->GetTransform().GetWorldPosition();

	auto enemyPos = this->GetOwner()->GetTransform().GetWorldPosition();
	auto textureSize = GetOwner()->GetComponent<SpriteComponent>()->GetTextureSize();
	auto scale = GetOwner()->GetTransform().GetScale();
	glm::vec3 bulletSpawnPosition{ enemyPos.x + textureSize.x / 2 * scale.x, enemyPos.y + textureSize.y * scale.y, 0 };

	auto bullet = ObjectFactory::GetInstance().CreateEnemyBullet(bulletSpawnPosition, playerPos);

	SceneManager::GetInstance().GetActiveScene().Add(bullet);
}
