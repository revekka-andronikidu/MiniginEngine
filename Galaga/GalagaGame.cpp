#pragma once
#include "GalagaGame.h"
//#include <windows.h>
#include <SDL.h>
#include <InputManager.h>
#include "ObjectFactory.h"
#include "GameCommands.h"
#include "SceneNames.h"
#include "GameEvents.h"
#include "Scene.h"

using namespace dae;

GalagaGame::GalagaGame() 
{
	//TODO:
	//init resources (textures, sounds, fonts)
	//init high scores
	//init game/level info


	//create all scenes in object factory
	//create all game states and keep reffence to them?? or create them when mode is entered

	
}
void GalagaGame::Update()
{

}

void GalagaGame::OnNotify(const GameObject& entity, const Event& event)
{
	if (event == OBJECT_DEATH_EVENT && entity.HasTag("player"))
	{
		m_StateMachine.EnterState<GameOverState>();
		m_GameModeMachine.EnterState<GameModeNull>();
	}

}

void GalagaGame::Initialize() //initialize after construction otherwise conflict in GameManager
{
	CreateScenes();
	m_GameModeMachine.EnterState<GameModeNull>();
	m_StateMachine.EnterState<MainMenuState>();
	
	SetDebugCommands();
}

void GalagaGame::SetDebugCommands()
{
	auto& input = dae::InputManager::GetInstance();

	auto skipLevel = std::make_unique<SkipLevelCommand>();
	input.BindGlobalInput(KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_F1 }, std::move(skipLevel));
	auto muteGame = std::make_unique<MuteGameCommand>();
	input.BindGlobalInput(KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_F2 }, std::move(muteGame));
}

void GalagaGame::SkipToNextStage()
{
	//check if the state is in game(not main menu)
	if (dynamic_cast<InGameState*>(m_StateMachine.GetCurrentState()))
	{
		m_CurrentStage = (m_CurrentStage + 1) % (m_NumberOfStages + 1);

		
		if (m_CurrentStage == 0)
		{
			m_StateMachine.EnterState<MainMenuState>();
			m_GameModeMachine.EnterState<GameModeNull>();
		}
		else
		{
			//next wave
		}

#if _DEBUG
		std::cout << "Stage Skipped. Current stage: " << std::to_string(m_CurrentStage) << std::endl;
#endif
	}
	else
	{
#if _DEBUG
		std::cout << "Stage cannor be skipped, not in game mode" << std::endl;
#endif
	}
}


void GalagaGame::EnterScene()
{

	
	
}

void GalagaGame::CreateScenes()
{
	dae::SceneManager::GetInstance().CreateScene(SceneNames::MainMenu);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::HighScores);
}