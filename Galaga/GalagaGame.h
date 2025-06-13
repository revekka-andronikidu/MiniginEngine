#pragma once
#include <Game.h>
#include "GameMode.h"
#include "GameStates.h"
#include <StateMachine.h> 
#include <iostream>
#include "Observer.h"


namespace dae
{
	class Scene;
	class GalagaGame final : public Game , public Observer
	{
	public:
		GalagaGame();
		virtual ~GalagaGame() = default;
		
		void Initialize() override;
		void Update() override;


		void SkipToNextStage();
		void EnterScene();

		const static int m_GameWindowWidth{ 600 };
		const static int m_GameWidnowHeight{ 600 };
		int m_CurrentStage{ 0 };
		const int m_NumberOfStages{ 3 };

		StateMachine<GameState> m_StateMachine;
		StateMachine<GameMode> m_GameModeMachine;

		void OnNotify(const GameObject& entity, const Event& event) override;

		void IncrementShotsFired() { m_ShotsFired++;  };
		void IncrementEnemiesHit() { m_Hits++; std::cout << std::to_string(m_Hits) << std::endl;
		};

	private:
		void CreateScenes();
		void SetDebugCommands();
		//void CreatePlayers(Scene& scene);

		//void Restet();
		
	
		int m_HighScore{ 0 };
		int m_Hits{0};
		int m_ShotsFired{0};
		
		

		
	};

}

