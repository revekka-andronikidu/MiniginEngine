#include "EnemyManager.h"
#include "EnemyComponent.h"
#include <GameObject.h>
#include "GameEvents.h"
#include <TimeManager.h>
#include "ObjectFactory.h"
#include "Helpers.h"
#include <SceneManager.h>
#include <Scene.h>
using namespace dae;

void EnemyManager::AddEnemy(EnemyType type)
{
	//m_EnemiesToRespawn.push_back(type);
	m_EnemyTypes.push_back( type );
}

void EnemyManager::StopAllEnemies()
{
	auto enemies = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag(Tag::ENEMY);
	for (auto enemy : enemies)
	{
		if (auto enemyComp = enemy->GetComponent<EnemyComponent>())
		{
			enemyComp->m_CanMove = false;
		}
	}

}

void EnemyManager::AddSpawnPoint(int x, int y)
{
	m_SpawnPoints.push_back({ x, y });
}

void EnemyManager::Restart()
{
	for (auto enemy : m_EnemyTypes)
	{
		m_EnemiesToRespawn.push_back(enemy);
	}
	for (auto enemy : m_Enemies)
	{
		EventManager::GetInstance().RemoveListener(this);
		enemy->Destroy();
	}
	m_Enemies.clear();
	m_LastSpaenPointIndex = 0;
	m_SpawnTimer = 1.0f;
	m_BatchTimer = 0.0f;
	m_SpawnBatchCount = 0;
}

void EnemyManager::NewLevel()
{
	m_Enemies.clear();
	m_EnemiesToRespawn.clear();
	m_SpawnPoints.clear();
	m_EnemyTypes.clear();
	m_pGrid = nullptr;
	m_LastSpaenPointIndex = 0;
	m_SpawnTimer = 1.0f;
	m_BatchTimer = 0.0f;
	m_SpawnBatchCount = 0;
	EventManager::GetInstance().RemoveListener(this);
}


void dae::EnemyManager::OnNotify(const GameObject& entity, const BaseEvent& event)
{

	if (auto evemt = dynamic_cast<const EnemyDefeatedEvent*>(&event))
	{

		m_EnemiesToRespawn.push_back(entity.GetComponent<EnemyComponent>()->GetType());
		//EventManager::GetInstance().RemoveListener<EnemyDefeatedEvent>(enemy.get(), this);
	}
}

void EnemyManager::Update()
{
	if (!m_EnemiesToRespawn.empty() && !m_SpawnPoints.empty())
		Spawn();
}

void EnemyManager::Spawn()
{
	m_SpawnTimer -= TimeManager::GetInstance().GetDeltaTime();
	m_BatchTimer += TimeManager::GetInstance().GetDeltaTime();

	// If no enemy spawned for long enough, reset batch
	if (m_BatchTimer >= m_BatchResetTime)
	{
		m_SpawnBatchCount = 0;
	}

	if (m_SpawnTimer <= 0.0f)
	{
		int enemiesToSpawn = 0;

		if (m_SpawnBatchCount == 0) {
			// First time -> spawn 2
			enemiesToSpawn = std::min(m_maxSpawnEnemiesAtOneTime, (int)m_EnemiesToRespawn.size());
		}
		else
		{
			// After that -> spawn 1
			enemiesToSpawn = 1;
		}

		for (int i = 0; i < enemiesToSpawn && !m_EnemiesToRespawn.empty(); ++i)
		{
			auto enemyType = m_EnemiesToRespawn.front();
			m_EnemiesToRespawn.erase(m_EnemiesToRespawn.begin());
			SpawnEnemy(enemyType, m_SpawnPoints[m_LastSpaenPointIndex]);
			m_LastSpaenPointIndex++;
			m_LastSpaenPointIndex %= m_SpawnPoints.size();
		}

		m_SpawnBatchCount += enemiesToSpawn;
		m_SpawnTimer = m_SpawnCooldown;
		m_BatchTimer = 0.0f; // reset "idle" timer
	}
}

void EnemyManager::SpawnEnemy(EnemyType type,const SpawnPoint& spawnpoint)
{
	glm::vec3 startPos; // Assuming glm::vec2 for positions

	constexpr int cellSize = GameSettings::cellSize * GameSettings::scale.x; // <-- replace with your tile size
	//constexpr int offsetCells = 2;
	startPos.x = spawnpoint.x * cellSize;
	startPos.y = spawnpoint.y * cellSize;

	auto enemy = ObjectFactory::GetInstance().CreateEnemy(type, startPos);
	m_Enemies.push_back(enemy.get());
	auto& scene = SceneManager::GetInstance().GetActiveScene();
	auto lvl = scene.GetObjectsWithTag(Tag::LEVEL);

	enemy.get()->SetParent(lvl[0], false);
	enemy->GetComponent<EnemyComponent>()->SetGrid(m_pGrid);
	EventManager::GetInstance().AddListener<EnemyDefeatedEvent>(enemy.get(), this);
	scene.Add(std::move(enemy));

}