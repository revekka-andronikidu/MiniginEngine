#pragma once
#include "GalagaGame.h"
//#include <windows.h>
#include <SDL.h>
#include <InputManager.h>
#include "ObjectFactory.h"
#include "GameCommands.h"
#include "SceneNames.h"
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

void GalagaGame::Initialize() //initialize after construction otherwise conflict in GameManager
{
	CreateScenes();
	m_StateMachine.EnterState<MainMenuState>();
	//EnterState(std::make_unique<MainMenuState>());
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
		}
		
		//Eneter correct state
		//init new state
		//state on enter should open the right scene

		//TEMP TEST
		//EnterScene();
		//SceneManager::GetInstance().SetActiveScene(GetSceneForCurrentState());
		//TEMP TEST
		//


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

void GalagaGame::EnterGameMode(std::unique_ptr<GameMode> state)
{
	//enter the state by name? keep vector of game states?
	//if doest exist create
	if (state)
	{
		m_currentGameMode->OnExit();
		m_currentGameMode = std::move(state);
		m_currentGameMode->OnEnter();
	}
}

std::string GalagaGame::GetSceneForCurrentState()
{
	switch (m_currentGameMode->GetModeType())
	{
	case GameMode::GameModeType::Solo:
		switch (m_CurrentStage)
		{
		case 1:
			return SceneNames::SoloStage1;
			break;
		case 2:
			return SceneNames::SoloStage2;
			break;
		case 3:
			return SceneNames::SoloStage3;
			break;
		default: 
			//handle error
			return SceneNames::MainMenu;
			break;
		}
		break;
	case GameMode::GameModeType::Coop:
		switch (m_CurrentStage)
		{
		case 1:
			return SceneNames::CoopStage1;
			break;
		case 2:
			return SceneNames::CoopStage2;
			break;
		case 3:
			return SceneNames::CoopStage3;
			break;
		default:
			//handle error
			return SceneNames::MainMenu;
			break;
		}
		break;

		break;
	case GameMode::GameModeType::Versus:
		switch (m_CurrentStage)
		{
		case 1:
			return SceneNames::VersusStage1;
			break;
		case 2:
			return SceneNames::VersusStage2;
			break;
		case 3:
			return SceneNames::VersusStage3;
			break;
		default:
			//handle error
			return SceneNames::MainMenu;
			break;
		}
		break;
	default:
		//handle error
		return SceneNames::MainMenu;
		break;
	}
	//create player/s

	//create hud, high score display, point display, lives display
	
}

//void GalagaGame::EnterStage()
//{
//	switch (m_CurrentStage)
//	{
//	case 1:
//		EnterState(std::make_unique<StageOneState>());
//		break;
//	case 2:
//		EnterState(std::make_unique<StageTwoState>());
//		break;
//	case 3:
//		EnterState(std::make_unique<StageThreeState>());
//		break;
//	default:
//		//handle error
//		EnterState(std::make_unique<MainMenuState>());
//		break;
//	}
//}

void GalagaGame::CreatePlayers(Scene& scene)
{
	
	if (m_PlayerOneLives > 0)
	{
		auto playerOne = ObjectFactory::GetInstance().CreatePlayer();
		//m_pPlayers.push_back(&playerOne);
		scene.Add(playerOne);
	}
	
	if (m_currentGameMode->GetModeType() == GameMode::GameModeType::Coop)
	{
		if (m_PlayerTwoLives > 0)
		{
			auto playerTwo = ObjectFactory::GetInstance().CreatePlayer();
			//m_pPlayers.push_back(&playerTwo);
		}
	}
	else if (m_currentGameMode->GetModeType() == GameMode::GameModeType::Versus)
	{
		
			auto playerTwo = ObjectFactory::GetInstance().CreatePlayer();
			//m_pPlayers.push_back(&playerTwo);
		
	}

}

void GalagaGame::EnterScene()
{

	auto& scene = SceneManager::GetInstance().GetScene(GetSceneForCurrentState());
	
	
	/*m_CurrentGameInfo = m_pGameInfoLoader->GetGameInfo(sceneName);
	auto& grid = GameObjectFactory::GetInstance().CreateGrid(scene, m_CurrentGameInfo);
	m_pGrid = grid.GetComponent<TriangularGrid>();

	GameObjectFactory::GetInstance().CreateHighScoreHUD(scene, m_HighScore);
	GameObjectFactory::GetInstance().CreateHUDLevel(scene, m_CurrentGameInfo.level);

	m_pEnemyManager->SetNewData(m_CurrentGameInfo);
	m_pEnemyManager->SetTriangularGrid(*m_pGrid);
	m_pEnemyManager->SetCurrentScene(&scene);

	m_pEnemyListener = std::make_shared<EnemyListener>(m_pEnemyManager.get());
	EventDispatcher::GetInstance().SubscribeListener(m_pEnemyListener);*/

	//CreatePlayers(scene);


	SceneManager::GetInstance().SetActiveScene(scene);
}

void GalagaGame::CreateScenes()
{
	dae::SceneManager::GetInstance().CreateScene(SceneNames::MainMenu);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::HighScores);
}