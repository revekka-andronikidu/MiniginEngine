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

		//Load from file
		const int kRows = 13; // (0–12 inclusive)
		const int kCols = 13; 
		////////

		int m_WindowWidth{};
		int m_WindowHeight{};
		glm::vec3 m_Scale{1.f, 1.f, 1.f};
		int m_CellSize{16};
		int m_HUDSize{32};

		unsigned short m_CurrentStage{ 0 };
		StateMachine<GameState> m_GameModeMachine;


		BurgerTimeGame();
		virtual ~BurgerTimeGame() = default;

		void Initialize(const GameInitParams& gameInitParms) override;
		void Update() override {};
		//void EnterCoopMode() { m_GameModeMachine.EnterState<CoopMode>(); };
		//void EnterVersusMode() { m_GameModeMachine.EnterState<VersusMode>(); };


		void LoadLevel();

		//init players
		//init enemies

	
	private:
		void SetDebugCommands();

		



		


	};
}

