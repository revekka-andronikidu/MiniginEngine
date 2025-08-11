#include "BurgerTimeGame.h"
#include <SDL.h>
#include <ResourceManager.h>
#include <ServiceLocator.h>
#include <Audio.h>
#include <InputManager.h>
#include "GameCommands.h"
#include "GameState.h"
#include "GameScenes.h"


#include "Helpers.h"
#include "SceneFactory.h"





using namespace dae;

BurgerTimeGame::BurgerTimeGame()
	: m_GameModeMachine{}
{
	std::unique_ptr<dae::IAudio> audio = std::make_unique<dae::SDLAudio>();
#if _DEBUG
	std::unique_ptr<dae::LoggerAudio> LogAudio = std::make_unique<dae::LoggerAudio>(std::move(audio));
	dae::ServiceLocator::RegisterAudioService(std::move(LogAudio));
#else
	dae::ServiceLocator::RegisterAudioService(std::move(audio));
#endif
	

	//TODO:
	//init ALL resources (textures, sounds, fonts)
	//FONTS
	ResourceManager::GetInstance().LoadFont("emulogic.ttf", 8);
	ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);

	ResourceManager::GetInstance().LoadTexture("spritesheet.png");

	//SOUNDS
	ResourceManager::GetInstance().LoadSound("BGM.wav"); //0
	//ServiceLocator::GetAudioService().PlayEffect(0, 0.8f, true);

	//init high scores


	m_GameModeMachine.EnterState<GameState>();
	SetDebugCommands();
}

void BurgerTimeGame::Initialize(const GameInitParams& gameInitParms)
{
	m_WindowWidth = gameInitParms.windowWidth;
	m_WindowHeight = gameInitParms.windowHeight;
	m_Scale = gameInitParms.scale;
	m_CellSize = gameInitParms.cellSize;
	m_HUDSize= gameInitParms.HUDSize;


	auto& scene = SceneManager::GetInstance().CreateScene(SceneNames::MainMenu);
	m_GameModeMachine.EnterState<MainMenuState>();
}

void BurgerTimeGame::SetDebugCommands()
{
	auto& input = dae::InputManager::GetInstance();

	//auto skipLevel = std::make_unique<SkipLevelCommand>();
	//input.BindGlobalInput(KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_F1 }, std::move(skipLevel));

	auto muteGame = std::make_unique<MuteGameCommand>();
	input.BindGlobalInput(KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_F2 }, std::move(muteGame));
}

void BurgerTimeGame::LoadLevel()
{
	// put t
	SceneFactory::GetInstance().CreateLevel(m_GameModeMachine.GetCurrentState()->GetModeType(), m_CurrentStage);
	
	
}
  