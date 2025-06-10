#pragma once
#include <Game.h>


namespace dae
{
	class GalagaGame final : public Game
	{
	public:
		GalagaGame();
		virtual ~GalagaGame() = default;

		void SkipStage();



	private:
		enum class GameMode
		{
			Single,
			Coop,
			Versus
		};


		void SetDebugCommands();
		//void LoadHighScores();
		// int GetHighestScore();
		//void CreateScenes();
		void CreateMainMenu();
		//void CreateScenes();
		void EnterSinglePlayer();
		void CreateSinglePlayerScene();
		void EnterCoop();
		void EnterVersus();
		void EnterHighScores();



		

		int m_PlayerOneLives{ 3 };
		int m_PlayerOnePoints{ 0 };
		int m_PlayerTwoLives{ 3 };
		int m_PlayerTwoPoints{ 0 };
		int m_HighScore{ 0 };
		int m_CurrentStage{ 0 };

		GameMode m_currentGameMode{ GameMode::Single };
	};

}

