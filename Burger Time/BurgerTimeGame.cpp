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
#include "LivesComponent.h"
#include "PlayerComponent.h"

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
	ResourceManager::GetInstance().LoadSound(SoundID::Death.filename);
	ResourceManager::GetInstance().LoadSound(SoundID::PepperShake.filename);
	ResourceManager::GetInstance().LoadSound(SoundID::EnemyPeppered.filename);
	ResourceManager::GetInstance().LoadSound(SoundID::EnemySqiushed.filename);
}
void BurgerTimeGame::AddListeners()
{

	EventManager::GetInstance().AddGlobalListener<LevelCompleteEvent>(this);
	EventManager::GetInstance().AddGlobalListener<PointsIncreasedEvent>(this);
	EventManager::GetInstance().AddGlobalListener<LivesUpdatedEvent>(this);
}

void BurgerTimeGame::Initialize()
{
	m_GameModeMachine.EnterState<MainMenuState>();
	//m_GameModeMachine.EnterState<GameOverState>(5.f);
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
	
};

void BurgerTimeGame::SkipLevel()
{
	if(IsGameplayActive())
	{
		if (m_CurrentStage < m_MaxStages)
		{
			m_GameModeMachine.EnterState<LevelCompleteState>(1.0f);
		}
		else
		{
			m_GameModeMachine.EnterState<GameOverState>(5.f);
		}	//display tutorial and play game start sound
	}
}

bool BurgerTimeGame::IsGameplayActive() const
{
	if (dynamic_cast<InGameState*>(m_GameModeMachine.GetCurrentState()))
	{
		return true;
	}
	return false;
}


void BurgerTimeGame::ResetGame()
{
	m_Score = 0;
	m_PlayerLives = 3;
	m_CurrentStage = 1;
	m_Peppers = 5; 
	m_SelectedGameMode = GameMode::Unknown;

	auto& hs = HighScoreManager::GetInstance();
	m_HighScore = hs.GetHighestScore();

	if (hs.GetScores().size() >= 5)
	{
		m_LowestSavedScore = hs.GetScores().back().score;
	}
}
void BurgerTimeGame::RestartStage()
{
	//player spawn
	// 
	Scene& scene = SceneManager::GetInstance().GetActiveScene();
	auto player = scene.GetObjectsWithTag(Tag::PLAYER);
	player[0]->GetTransform().SetPosition(m_playerStartPosition);
	player[0]->GetComponent<PlayerComponent>()->ResetPlayer(m_playerStartPosition);
	//RestartEnemyManager

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

	if (auto livesEvent = dynamic_cast<const LivesUpdatedEvent*>(&event))
	{
		// Update the health bar UI
		m_PlayerLives = entity.GetComponent<LivesComponent>()->GetLives();

		m_GameModeMachine.EnterState<PlayerDeathState>(3.f);

	}
}