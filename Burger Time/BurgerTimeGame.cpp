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
#include "HighScoresManager.h"


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
	
	auto& hs = HighScoreManager::GetInstance();
	hs.Init("highscores.json"); 
	m_HighScore = hs.GetHighestScore();

	if (hs.GetScores().size() >= 5)
	{
		m_LowestSavedScore = hs.GetScores().back().score;
	}
	
	


	LoadResources();
	AddListeners();
	SetDebugCommands();


	SceneManager::GetInstance().CreateScene(SceneNames::DummyScene);
	SceneManager::GetInstance().SetActiveScene(SceneNames::DummyScene);	
	m_GameModeMachine.EnterState<GameState>();
}

void BurgerTimeGame::LoadResources()
{
	ResourceManager::GetInstance().LoadFont("emulogic.ttf", 8);

	ResourceManager::GetInstance().LoadTexture("spritesheet.png");

	//SOUNDS
	ResourceManager::GetInstance().LoadSound(SoundID::BGM.filename);
	ResourceManager::GetInstance().LoadSound(SoundID::BurgerFall.filename);
	ResourceManager::GetInstance().LoadSound(SoundID::BurgerLand.filename);
	ResourceManager::GetInstance().LoadSound(SoundID::BurgerStep.filename);
	ResourceManager::GetInstance().LoadSound(SoundID::RoundClear.filename);
	ResourceManager::GetInstance().LoadSound(SoundID::GameStart.filename);
	ResourceManager::GetInstance().LoadSound(SoundID::SystemSound.filename);
}
void BurgerTimeGame::AddListeners()
{

	EventManager::GetInstance().AddGlobalListener<LevelCompleteEvent>(this);
	EventManager::GetInstance().AddGlobalListener<PointsIncreasedEvent>(this);
}

void BurgerTimeGame::Initialize()
{
	//m_GameModeMachine.EnterState<MainMenuState>();
	m_GameModeMachine.EnterState<GameOverState>(5.f);
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
	//m_EnemyScore = 0; 
	m_CurrentStage = 1;
	m_Peppers = 5; 

	
}
  

void BurgerTimeGame::OnNotify(const GameObject& entity, const BaseEvent& event)
{
	if (dynamic_cast<const LevelCompleteEvent*>(&event))
	{
					//stop all enemy movement
					// //stop player 
					//play animation
					//move to next stage

		if (m_CurrentStage < m_MaxStages)
		{
			m_GameModeMachine.EnterState<LevelCompleteState>(4.0f);
			return;
		}
		else
		{
			m_GameModeMachine.EnterState<GameOverState>(5.f);			
		}
		
	}

	if (auto pointsEvent = dynamic_cast<const PointsIncreasedEvent*>(&event))
	{
		m_Score += pointsEvent->GetPoints();
		GameObject dummySender;
		EventManager::GetInstance().TriggerEvent(ScoreUpdatedEvent{ m_Score }, dummySender);
	}
	


}