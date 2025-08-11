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
#include "EnemyManager.h"
#include "ServiceLocator.h"
#include "Audio.h"
#include "SceneFactory.h"

using namespace dae;

GalagaGame::GalagaGame() 
{


	//Load audio
	std::unique_ptr<IAudio> audio = std::make_unique<SDLAudio>();

#if _DEBUG
	std::unique_ptr<LoggerAudio> LogAudio = std::make_unique<LoggerAudio>(std::move(audio));
	ServiceLocator::RegisterAudioService(std::move(LogAudio));
#else
	ServiceLocator::RegisterAudioService(std::move(audio));
#endif

	//TODO:
	//init ALL resources (textures, sounds, fonts)
	
	ResourceManager::GetInstance().LoadSound("GalagaTheme.wav"); //0
	ResourceManager::GetInstance().LoadSound("Fighter-Shot.wav"); //1
	ResourceManager::GetInstance().LoadSound("Galaga-explosion.wav"); //2
	ResourceManager::GetInstance().LoadSound("Galaga-kill.wav"); //3

	//init high scores
	
}
void GalagaGame::Update()
{
	m_EnemyManager->Update();
}
void GalagaGame::Reset()
{
	m_ShotsFired = 0;
	m_Hits = 0;
}

void GalagaGame::OnNotify(const GameObject& entity, const Event& event)
{
	if (event == OBJECT_DEATH_EVENT && entity.HasTag("player"))
	{
		m_GameModeMachine.EnterState<GameOverState>();
	}

}

void GalagaGame::Initialize(const GameInitParams& gameInitParms) //initialize after construction otherwise conflict in GameManager
{
	CreateScenes();
	m_GameModeMachine.EnterState<MainMenuState>();
	m_EnemyManager = std::make_unique<EnemyManager>();
	SetDebugCommands();
}

void GalagaGame::SetDebugCommands()
{
	auto& input = dae::InputManager::GetInstance();

	auto skipLevel = std::make_unique<SkipLevelCommand>();
	input.BindGlobalInput(KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_F1 }, std::move(skipLevel));
	auto muteGame = std::make_unique<MuteGameCommand>();
	input.BindGlobalInput(KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_F2 }, std::move(muteGame));
}

void GalagaGame::SkipToNextStage()
{
	//check if the state is in game(not main menu)
	if (m_GameModeMachine.GetCurrentState()->GetModeType() != GameMode::GameModeType::Unknown)
	{
		



#if _DEBUG
		std::cout << "Stage Skipped. Current stage: " << std::to_string(0) << std::endl;
#endif
	}
	else
	{
#if _DEBUG
		std::cout << "Stage cannor be skipped, not in game mode" << std::endl;
#endif
	}
}




void GalagaGame::CreateScenes()
{
	dae::SceneManager::GetInstance().CreateScene(SceneNames::MainMenu);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::HighScores);
}