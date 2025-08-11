#pragma once
#include <Game.h>
#include <StateMachine.h>
#include "GameState.h"
#include "Helpers.h"

namespace dae
{
	class Grid;
	class BurgerTimeGame final : public Game
	{
	public:

		//Load from file or put in game settings
		const int kRows = 13; // (0–12 inclusive)
		const int kCols = 13; 
		////////

		int m_Score{ 0 };
		int m_EnemyScore{ 0 }; //if versus mode

		unsigned short m_CurrentStage{ 0 };
		StateMachine<GameState> m_GameModeMachine;


		BurgerTimeGame();
		virtual ~BurgerTimeGame() = default;

		void Initialize() override;
		void Update() override {};


		void LoadLevel();

	private:
		void SetDebugCommands();
	};
}

