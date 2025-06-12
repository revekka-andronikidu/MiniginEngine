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
		void EnterState(std::unique_ptr<GameState> state);
		void EnterGameMode(std::unique_ptr<GameMode> state);
		GameMode* GetCurrentGameMode() { return m_currentGameMode.get(); };
		void EnterScene();

		const static int m_GameWindowWidth{ 480 };
		const static int m_GameWidnowHeight{ 512 };
		int m_CurrentStage{ 0 };
		const int m_NumberOfStages{ 3 };

		dae::StateMachine<GameState> m_StateMachine;

	private:
		void CreateScenes();
		void SetDebugCommands();
		void CreatePlayers(Scene& scene);
		//void EnterStage();
		//void LoadHighScores();
		// int GetHighestScore();

		//void EnterHighScores();
		//void Restet();
		
		
		
		int m_PlayerOneLives{ 3 };
		int m_PlayerOnePoints{ 0 };
		int m_PlayerTwoLives{ 3 };
		int m_PlayerTwoPoints{ 0 };
		int m_HighScore{ 0 };
		
		

		//GameMode m_currentGameMode{ GameMode::Single };
		

		//std::unique_ptr<GameState> m_currentGameState{std::make_unique<GameState>()};
		std::unique_ptr<GameMode> m_currentGameMode{ std::make_unique<GameModeNull>() };
	};

}

