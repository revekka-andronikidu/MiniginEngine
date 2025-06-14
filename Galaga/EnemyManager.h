#pragma once
#include <string>
#include <vector>
#include <vec2.hpp>

namespace dae
{
	class EnemyManager final 
	{

	public:
		EnemyManager();
		~EnemyManager() = default;
		EnemyManager(const EnemyManager& other) = delete;
		EnemyManager(EnemyManager&& other) = delete;
		EnemyManager& operator=(const EnemyManager& other) = delete;
		EnemyManager& operator=(EnemyManager&& other) = delete;

		void LoadPathsFromFile(const std::string& filename);
		void StartStage(int stage);

	private:
		struct Path 
		{
			std::string name;
			std::vector<glm::vec2> points;
		};

		struct EnemySpawnInfo
		{
			std::string type;
			int x;
			int y;
			float moveDistance;
			int order;
			int subOrder;
			std::vector<glm::vec2> path;
			bool spawnTogether;
		};


		int m_Stages;
		int m_CurrentStage{ 0 };

		int m_CurrentGroup = 0;
		int m_EnemiesInCurrentStage = 0;

		std::vector<Path> m_Paths;

		//m_Waves{};

		
	};

	
}

