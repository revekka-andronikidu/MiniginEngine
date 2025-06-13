#include "GameStates.h"
#include "SceneNames.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "GalagaGame.h"
#include "SceneFactory.h"

using namespace dae;


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

	//galaga->EnterScene();
	sceneManager.SetActiveScene(SceneNames::MainMenu);
	
}
void MainMenuState::OnExit()
{ 
	//SceneManager::GetInstance().DestroyScene(SceneNames::MainMenu); 
	//m_menuCreated = false;
}

void HighScoresState::OnEnter()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto galaga = dynamic_cast<GalagaGame*>(game);
	auto& sceneManager = dae::SceneManager::GetInstance();


	if (!m_menuCreated)
	{
		//auto& scene = sceneManager.GetScene(SceneNames::MainMenu);
		//SceneFactory::GetInstance().CreateMainMenu();
		m_menuCreated = true;
	}

	auto& scene = SceneManager::GetInstance().GetScene(SceneNames::HighScores);
	SceneManager::GetInstance().SetActiveScene(scene);
	//sceneManager.SetActiveScene(SceneNames::MainMenu);

}

void WaveOneState::OnEnter()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto galaga = dynamic_cast<GalagaGame*>(game);
	galaga->m_CurrentStage = 1;


	// Get scene name based on game mode
	
	SceneFactory::GetInstance().CreateStageScene(SceneNames::Solo);
	dae::SceneManager::GetInstance().SetActiveScene(SceneNames::Solo);

	//galaga->EnterScene(); // Sets active scene
	//ResetStage(sceneName); // Reset enemies/players if needed

}




void GameOverState::OnEnter()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto galaga = dynamic_cast<GalagaGame*>(game);

	SceneFactory::GetInstance().CreateGameOverScene();
	SceneManager::GetInstance().SetActiveScene(SceneNames::GameOver);


}


