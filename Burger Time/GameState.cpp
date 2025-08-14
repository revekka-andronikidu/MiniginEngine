#include "GameState.h"
#include <SceneManager.h>
#include <ServiceLocator.h>
#include "SceneFactory.h"
#include "GameManager.h"
#include "BurgerTimeGame.h"
#include "TimeManager.h"
#include "TrayManager.h"
#include <Scene.h>
using namespace dae;

void MainMenuState::OnEnter()
{	
	auto& sceneManager = dae::SceneManager::GetInstance();

	if (!sceneManager.HasScene(SceneNames::MainMenu))
	{
		SceneFactory::GetInstance().CreateMainMenu();
	}
	sceneManager.QueueSceneChange(SceneNames::MainMenu, SceneManager::GetInstance().GetActiveScene().GetSceneName());
}
void MainMenuState::OnExit()
{

	//ServiceLocator::GetAudioService().StopSound(SoundID::BGM.id);
	//SceneManager::GetInstance().DestroyScene(SceneNames::MainMenu); 
	//m_menuCreated = false;
}

void InGameState::OnEnter()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto burgerTime = dynamic_cast<BurgerTimeGame*>(game);

	auto& scene = SceneManager::GetInstance().CreateScene(SceneNames::GetStageName(burgerTime->m_CurrentStage));
	SceneFactory::GetInstance().CreateLevel(burgerTime->m_CurrentStage);

	if (burgerTime->m_CurrentStage == 1)
	{
		SceneManager::GetInstance().SetActiveScene(SceneNames::Stage1);
	}
	else
	{
		SceneManager::GetInstance().QueueSceneChange(SceneNames::GetStageName(burgerTime->m_CurrentStage), SceneNames::GetStageName(burgerTime->m_CurrentStage - 1));
	}

	ServiceLocator::GetAudioService().PlayEffect(SoundID::BGM.id, 0.4f, true);
}

void InGameState::OnExit()
{
	ServiceLocator::GetAudioService().StopSound(SoundID::BGM.id);
}


void GameState::NextStage()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto burgerTime = dynamic_cast<BurgerTimeGame*>(game);

	burgerTime->m_CurrentStage += 1;
	TrayManager::GetInstance().Reset();
	burgerTime->m_GameModeMachine.EnterState<InGameState>(burgerTime->m_SelectedGameMode);
}

void LevelCompleteState::OnEnter()
{
	ServiceLocator::GetAudioService().PlayEffect(SoundID::RoundClear.id, 0.8f, false);
	m_Timer = 0.f;
	//StopAllEnemies(); in enemy manager
	//PlayCompletionAnimation();
		//in player, stop input and play custom animation
}

void LevelCompleteState::Update()
{
	m_Timer += TimeManager::GetInstance().GetDeltaTime();
	if (m_Timer >= m_WaitTime)
	{
		m_Timer = 0.f;
		NextStage();
	}
}
void LevelCompleteState::OnExit()
{
	ServiceLocator::GetAudioService().StopSound(SoundID::RoundClear.id);
	m_Timer = 0.f;
}

void HighScoresState::OnEnter()
{
	auto& scene = SceneManager::GetInstance().CreateScene(SceneNames::HighScores);
	SceneFactory::GetInstance().CreateHighScores();

	SceneManager::GetInstance().QueueSceneChange(SceneNames::HighScores, SceneManager::GetInstance().GetActiveScene().GetSceneName());
	//SceneManager::GetInstance().SetActiveScene(scene);
}

void HighScoresState::OnExit()
{

	SceneManager::GetInstance().QueueSceneChange(SceneNames::DummyScene, SceneNames::HighScores);
}

void HighScoreEnterState::OnEnter()
{
	ServiceLocator::GetAudioService().PlayEffect(SoundID::GameStart.id, 0.8f, false);

	auto& scene = SceneManager::GetInstance().CreateScene(SceneNames::HighScoreEntry);
	SceneFactory::GetInstance().CreateHighScoreEntry();

	SceneManager::GetInstance().SetActiveScene(scene);
}

void HighScoreEnterState::OnExit()
{
	SceneManager::GetInstance().QueueSceneChange(SceneNames::DummyScene, SceneNames::HighScoreEntry);
}

void GameOverState::OnEnter()
{
	SceneFactory::GetInstance().CreateGameOver();

	SceneManager::GetInstance().QueueSceneChange(SceneNames::GameOver, SceneManager::GetInstance().GetActiveScene().GetSceneName());
}

void GameOverState::Update()
{
	m_Timer += TimeManager::GetInstance().GetDeltaTime();
	if (m_Timer >= m_WaitTime)
	{
		m_Timer = 0.f;
		NextScreen();
	}
}

void GameOverState::NextScreen()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto burgerTime = dynamic_cast<BurgerTimeGame*>(game);


	if (burgerTime->m_Score > burgerTime->m_LowestSavedScore)
	{
		burgerTime->m_GameModeMachine.EnterState<HighScoreEnterState>();
	}
	else
	{
		burgerTime->m_GameModeMachine.EnterState<HighScoresState>();
	}
}


void GameOverState::OnExit()
{

	SceneManager::GetInstance().QueueSceneChange(SceneNames::DummyScene, SceneNames::GameOver);
}