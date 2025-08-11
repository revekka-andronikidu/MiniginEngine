#pragma once
#include <Game.h>
#include "GameMode.h"
#include <StateMachine.h> 
#include <iostream>
#include "Observer.h"
#include "EnemyManager.h"



namespace dae
{
	class Scene;


	class GalagaGame final : public Game , public Observer
	{
	public:
		GalagaGame();
		virtual ~GalagaGame() = default;
		
		void Initialize(const GameInitParams& gameInitParms) override;
		void Update() override;


		void SkipToNextStage();

		const static int m_GameWindowWidth{ 600 };
		const static int m_GameWidnowHeight{ 600 };
		const int m_NumberOfStages{ 3 };

		
		StateMachine<GameMode> m_GameModeMachine;
		std::unique_ptr<EnemyManager> m_EnemyManager;

		void OnNotify(const GameObject& entity, const Event& event) override;

		void IncrementShotsFired() { m_ShotsFired++;  };
		void IncrementEnemiesHit() { m_Hits++; };

		int GetShotsFired() const { return m_ShotsFired; };
		int GetEnemiesHit() const { return m_Hits; };


	private:
		void CreateScenes();
		void SetDebugCommands();
		//void CreatePlayers(Scene& scene);

		void Reset();
		
	
		int m_HighScore{ 0 };
		int m_Hits{0};
		int m_ShotsFired{0};

		
		
		

		
	};

}

