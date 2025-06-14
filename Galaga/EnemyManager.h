#pragma once
#include <string>
#include <vector>
#include <vec2.hpp>

namespace dae
{
	class GameObject;

	struct FormationEnemyInfo
	{
		std::string type;
		glm::vec2 position;
		int group;
		int subGroup;
		std::string pathName;
	};

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
		void StartAttack();

		void Update();



	private:

		struct Path 
		{
			std::string name;
			std::vector<glm::vec2> points;
		};

		std::vector<FormationEnemyInfo> LoadFormationFromFile(const std::string& filename);




		int m_Stages;
		int m_CurrentStage{ 0 };

		bool m_IsAttacking{false};
		float m_AttackTimmer{0};
		float m_AttackInterval{6.f};
		

		int m_CurrentGroup = 0;
		int m_EnemiesInCurrentStage = 0;
		int m_MaxAttackersAtOnce;
		std::vector<dae::GameObject*> m_Attackers;
		std::vector<Path> m_Paths;

		const int kGridCols = 10;
		const int kGridRows = 5;
		const int kTopOffset = 60;          // Top padding in pixels
		const int kVerticalSpacing = 60;     // Distance between rows
		const int kHorizontalPadding = 40;   // Padding on left/right

		int m_WindowHeight{};
		int m_WindowWidth{};


		std::vector<dae::GameObject*> SelectAttackers(const std::vector<dae::GameObject*>& enemies) const;
		glm::vec2 GridToWorldPosition(int col, int row);
		//m_Waves{};

		
	};

	
}

