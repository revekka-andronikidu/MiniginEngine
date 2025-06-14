#include "GameMode.h"
#include "SceneManager.h"
#include "SceneNames.h"
#include "GameManager.h"
#include "GalagaGame.h"
#include "GameStates.h"
#include "ServiceLocator.h"
#include "SceneFactory.h"

using namespace dae;

void SinglePlayerMode::OnEnter()
{
	//auto game = GameManager::GetInstance().GetActiveGame();
	//auto galaga = dynamic_cast<GalagaGame*>(game);

	if (!m_ScenesCreated)
	{
		CreateScenes();
		SceneFactory::GetInstance().CreateStageScene(SceneNames::Solo);

		m_ScenesCreated = true;
	}
	else
	{
		//Reset();
	}

	dae::SceneManager::GetInstance().SetActiveScene(SceneNames::Solo);

}

void VersusMode::OnEnter()
{
	//auto game = GameManager::GetInstance().GetActiveGame();
	//auto galaga = dynamic_cast<GalagaGame*>(game);

	if (!m_ScenesCreated)
	{
		CreateScenes();
		SceneFactory::GetInstance().CreateStageScene(SceneNames::Versus);

		m_ScenesCreated = true;
	}
	else
	{
		//Reset();
	}

	dae::SceneManager::GetInstance().SetActiveScene(SceneNames::Versus);

}

void CoopMode::OnEnter()
{
	//auto game = GameManager::GetInstance().GetActiveGame();
	//auto galaga = dynamic_cast<GalagaGame*>(game);

	if (!m_ScenesCreated)
	{
		CreateScenes();
		SceneFactory::GetInstance().CreateStageScene(SceneNames::Coop);

		m_ScenesCreated = true;
	}
	else
	{
		//Reset();
	}

	dae::SceneManager::GetInstance().SetActiveScene(SceneNames::Coop);

}


void SinglePlayerMode::CreateScenes()
{
	dae::SceneManager::GetInstance().CreateScene(SceneNames::Solo);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::StageScore);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::GameOver);
}



void CoopMode::CreateScenes()
{
	dae::SceneManager::GetInstance().CreateScene(SceneNames::Coop);;
	dae::SceneManager::GetInstance().CreateScene(SceneNames::StageScore);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::GameOver);
}

void VersusMode::CreateScenes()
{
	dae::SceneManager::GetInstance().CreateScene(SceneNames::Versus);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::StageScore);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::GameOver);
}

void MainMenuState::OnEnter()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto galaga = dynamic_cast<GalagaGame*>(game);
	auto& sceneManager = dae::SceneManager::GetInstance();


	if (!m_menuCreated)
	{
		//auto& scene = sceneManager.GetScene(SceneNames::MainMenu);
		SceneFactory::GetInstance().CreateMainMenu();
		m_menuCreated = true;
	}

	ServiceLocator::GetAudioService().PlayEffect(0, 0.8f, true);
	// 
	//galaga->EnterScene();
	sceneManager.SetActiveScene(SceneNames::MainMenu);

}
void MainMenuState::OnExit()
{

	ServiceLocator::GetAudioService().StopSound(0);
	//SceneManager::GetInstance().DestroyScene(SceneNames::MainMenu); 
	//m_menuCreated = false;
}

void HighScoresState::OnEnter()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto galaga = dynamic_cast<GalagaGame*>(game);
	auto& sceneManager = dae::SceneManager::GetInstance();


	auto& scene = SceneManager::GetInstance().GetScene(SceneNames::HighScores);
	SceneManager::GetInstance().SetActiveScene(scene);
}


void GameOverState::OnEnter()
{

	SceneFactory::GetInstance().CreateGameOverScene();
	SceneManager::GetInstance().SetActiveScene(SceneNames::GameOver);


}
