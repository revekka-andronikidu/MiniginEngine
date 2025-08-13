#include "BurgerTimeGame.h"
#include <SDL.h>
#include <ResourceManager.h>
#include <ServiceLocator.h>
#include <Audio.h>
#include <InputManager.h>
#include "GameCommands.h"
#include "GameState.h"

#include "Helpers.h"
#include "SceneFactory.h"

#include "GameEvents.h"


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
	ResourceManager::GetInstance().LoadSound(SoundID::BGM.filename); 
	ResourceManager::GetInstance().LoadSound(SoundID::BurgerFall.filename);  
	ResourceManager::GetInstance().LoadSound(SoundID::BurgerLand.filename);  
	ResourceManager::GetInstance().LoadSound(SoundID::BurgerStep.filename);  
	ResourceManager::GetInstance().LoadSound(SoundID::RoundClear.filename);  
	ResourceManager::GetInstance().LoadSound(SoundID::GameStart.filename);   
	ResourceManager::GetInstance().LoadSound(SoundID::SystemSound.filename); 

	//ServiceLocator::GetAudioService().PlayEffect(0, 0.8f, true);

	//init high scores

	EventManager::GetInstance().AddGlobalListener<LevelCompleteEvent>(this);
	EventManager::GetInstance().AddGlobalListener<PointsIncreasedEvent>(this);


	m_GameModeMachine.EnterState<GameState>();
	SetDebugCommands();

}

void BurgerTimeGame::Initialize()
{
	auto& scene = SceneManager::GetInstance().CreateScene(SceneNames::MainMenu);
	m_GameModeMachine.EnterState<MainMenuState>();
}

void BurgerTimeGame::SetDebugCommands()
{
	auto& input = dae::InputManager::GetInstance();

	auto skipLevel = std::make_unique<SkipLevelCommand>();
	input.BindGlobalInput(KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_F1 }, std::move(skipLevel));

	auto muteGame = std::make_unique<MuteGameCommand>();
	input.BindGlobalInput(KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_F2 }, std::move(muteGame));
}

void BurgerTimeGame::Update()
{

	m_GameModeMachine.Update();
	//if (m_GameModeMachine.GetCurrentState() == nullptr) return;
	//auto state = dynamic_cast<InGameState*>(m_GameModeMachine.GetCurrentState());
	//if (state)
	//{
	//	state->Update();
	//}
};

void BurgerTimeGame::GameStart()
{
	m_CurrentStage = 1;
	//display tutorial and play game start sound

}

void BurgerTimeGame::Reset()
{
	m_Score = 0;
	m_EnemyScore = 0; 
	m_CurrentStage = 1;
	m_Peppers = 5; 

	
}
  

void BurgerTimeGame::OnNotify(const GameObject& entity, const BaseEvent& event)
{
	if (dynamic_cast<const LevelCompleteEvent*>(&event))
	{
					//stop all enemy movement
					//play animation
					//move to next stage

		if (m_CurrentStage <= m_MaxStages)
		{
			m_GameModeMachine.EnterState<LevelCompleteState>(3.0f);
			return;
		}
		else
		{
			//m_GameModeMachine.EnterState<GameWon>();
			//write down high scores
			
		}
		
	}

	if (auto pointsEvent = dynamic_cast<const PointsIncreasedEvent*>(&event))
	{
		m_Score += pointsEvent->GetPoints();
		GameObject dummySender;
		EventManager::GetInstance().TriggerEvent(ScoreUpdatedEvent{ m_Score }, dummySender);
	}
	


}