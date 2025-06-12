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

void StageOneState::OnEnter()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto galaga = dynamic_cast<GalagaGame*>(game);

	galaga->m_CurrentStage = 1;
	galaga->EnterScene();

	//Enter / Create Scene
	
	
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
void StageOneState::CreateScene()
{
	//function in Galaga Game that loads the info from file 

	///*void QBertGameMode::LoadScene(const std::string sceneName)
	//{
	//	std::cout << "QBertGameMode started scene " << sceneName << std::endl;
	//	auto& sm = SceneManager::GetInstance();
	//	auto& scene = sm.CreateScene(sceneName);
	//	sm.SetActiveScene(scene);


	//	m_CurrentGameInfo = m_pGameInfoLoader->GetGameInfo(sceneName);
	//	auto& grid = GameObjectFactory::GetInstance().CreateGrid(scene, m_CurrentGameInfo);
	//	m_pGrid = grid.GetComponent<TriangularGrid>();

	//	GameObjectFactory::GetInstance().CreateHighScoreHUD(scene, m_HighScore);
	//	GameObjectFactory::GetInstance().CreateHUDLevel(scene, m_CurrentGameInfo.level);

	//	m_pEnemyManager->SetNewData(m_CurrentGameInfo);
	//	m_pEnemyManager->SetTriangularGrid(*m_pGrid);
	//	m_pEnemyManager->SetCurrentScene(&scene);

	//	m_pEnemyListener = std::make_shared<EnemyListener>(m_pEnemyManager.get());
	//	EventDispatcher::GetInstance().SubscribeListener(m_pEnemyListener);

	//	CreatePlayers(scene);
	//}*/

}