#pragma once
#include <Singleton.h>
#include <vector>
#include <EventManager.h>

namespace dae
{
    class GameObject;
	class EnemyComponent;
    enum class EnemyType;
	class GridComponent;
    class EnemyManager final : public dae::Singleton<EnemyManager>, public IEventListener
    {

    private:
        struct SpawnPoint 
        {
            int x;
            int y;
        };


        std::vector<GameObject*> m_Enemies{};
		std::vector<SpawnPoint> m_SpawnPoints{};
        std::vector<EnemyType> m_EnemyTypes{};
        std::vector<EnemyType> m_EnemiesToRespawn{};
        int m_LastSpaenPointIndex{0};
        int m_maxSpawnEnemiesAtOneTime{ 2 };
        float m_SpawnCooldown{ 1.5f };   // cooldown between spawns
        float m_SpawnTimer{ 1.0f };

        int m_SpawnBatchCount{ 0 };      // how many enemies spawned in this "batch"
        float m_BatchResetTime{ 4.0f };  // after 5s with no spawns, reset rule
        float m_BatchTimer{ 0.0f };
		GridComponent* m_pGrid{ nullptr };

		void SpawnEnemy(EnemyType type, const SpawnPoint& spawnPoint);
        void AddSpawnEnemy();

    public:
        void AddEnemy(EnemyType type);
        void StopAllEnemies();
        void Restart();
		void NewLevel();
        void AddSpawnPoint(int x, int y);
        void OnNotify(const GameObject& entity, const BaseEvent& event) override;
        void Update();
        void Spawn();
		void SetGrid(GridComponent* grid) { m_pGrid = grid; };
		
    };
}

