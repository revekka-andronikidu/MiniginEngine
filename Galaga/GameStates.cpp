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

	galaga->EnterScene();
	//sceneManager.SetActiveScene(SceneNames::MainMenu);
	
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

void StageOneState::OnEnter()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto galaga = dynamic_cast<GalagaGame*>(game);
	galaga->m_CurrentStage = 1;


	// Get scene name based on game mode
	const std::string sceneName = galaga->GetSceneForCurrentState();
	SceneFactory::GetInstance().CreateStageScene(sceneName); 
	

	galaga->EnterScene(); // Sets active scene
	//ResetStage(sceneName); // Reset enemies/players if needed

}
void StageOneState::CreateScene()
{
	//auto game = GameManager::GetInstance().GetActiveGame();
	//auto galaga = dynamic_cast<GalagaGame*>(game);


	//SceneManager::CreateScene()
}

void StageTwoState::OnEnter()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto galaga = dynamic_cast<GalagaGame*>(game);

	galaga->m_CurrentStage = 2;
	galaga->EnterScene();

	//Enter / Create Scene


}

void StageThreeState::OnEnter()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto galaga = dynamic_cast<GalagaGame*>(game);

	galaga->m_CurrentStage = 3;
	galaga->EnterScene();

	//Enter / Create Scene


}

//void StageScreenState::OnEnter()
//{
//	auto game = GameManager::GetInstance().GetActiveGame();
//	auto galaga = dynamic_cast<GalagaGame*>(game);
//
//	//get game stage
//
//	//get game mode 
//
//
//}
//

