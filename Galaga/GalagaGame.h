#pragma once
#include <Game.h>
#include "GameMode.h"
#include "GameStates.h"
#include <StateMachine.h> 


namespace dae
{
	class Scene;
	class GalagaGame final : public Game
	{
	public:
		GalagaGame();
		virtual ~GalagaGame() = default;
		
		void Initialize() override;


		
		std::string GetSceneForCurrentState();

		void SkipToNextStage();
		void EnterScene();

		const static int m_GameWindowWidth{ 600 };
		const static int m_GameWidnowHeight{ 600 };
		int m_CurrentStage{ 0 };
		const int m_NumberOfStages{ 3 };

		StateMachine<GameState> m_StateMachine;
		StateMachine<GameMode> m_GameModeMachine;

	private:
		void CreateScenes();
		void SetDebugCommands();
		//void CreatePlayers(Scene& scene);

		//void Restet();
		
	
		int m_HighScore{ 0 };
		
		

		//GameMode m_currentGameMode{ GameMode::Single };
		

		//std::unique_ptr<GameState> m_currentGameState{std::make_unique<GameState>()};
		//std::unique_ptr<GameMode> m_currentGameMode{ std::make_unique<GameModeNull>() };
	};

}

