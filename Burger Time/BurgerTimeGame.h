#pragma once
#include <Game.h>
#include <StateMachine.h>
#include "GameState.h"
#include "Helpers.h"
#include <EventManager.h>

namespace dae
{
	class Grid;
	class BurgerTimeGame final : public Game, public IEventListener
	{
	public:

		//Load from file or put in game settings
		const int kRows = 13; // (0–12 inclusive)
		const int kCols = 13; 

		const int m_MaxStages = 3;
		////////

		int m_Score{ 0 };
		int m_EnemyScore{ 0 }; //if versus mode
		int m_PlayerLives{ 3 };
		int m_Peppers{ 5 }; //default peppers
		//int m_EnemyPlayerLives{ 3 }; //if versus mode

		unsigned short m_CurrentStage{ 1 };
		StateMachine<GameState> m_GameModeMachine;
		GameMode m_SelectedGameMode{ GameMode::Unknown };


		BurgerTimeGame();
		virtual ~BurgerTimeGame() = default;

		void Initialize() override;
		void Update() override;


		void OnNotify(const GameObject& entity, const BaseEvent& event) override;

		void Reset();
		void GameStart();
		//void GameOver();
		//void StageWon();
		//void GameWon();

	private:
		void SetDebugCommands();
	};
}

