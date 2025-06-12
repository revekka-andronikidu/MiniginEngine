#include "GameMode.h"
#include "SceneManager.h"
#include "SceneNames.h"
#include "GameManager.h"
#include "GalagaGame.h"
#include "GameStates.h"

using namespace dae;

void GameMode::OnEnter()
{
	//create all scenes for single player
	if (!m_ScenesCreated)
	{
		CreateScenes();
		m_ScenesCreated = true;
	}

	auto game = GameManager::GetInstance().GetActiveGame();
	auto galaga = dynamic_cast<GalagaGame*>(game);

	//reset to start from the start, init enemies or reset

	galaga->m_StateMachine.EnterState<StageOneState>();
	
}

void SinglePlayerMode::CreateScenes()
{
	dae::SceneManager::GetInstance().CreateScene(SceneNames::SoloStage1);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::SoloStage2);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::SoloStage3);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::GameOver);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::StageX);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::StageScore);
}


void CoopMode::CreateScenes()
{
	dae::SceneManager::GetInstance().CreateScene(SceneNames::CoopStage1);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::CoopStage2);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::CoopStage3);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::GameOver);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::StageX);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::StageScore);
}

void VersusMode::CreateScenes()
{
	dae::SceneManager::GetInstance().CreateScene(SceneNames::VersusStage1);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::VersusStage2);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::VersusStage3);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::GameOver);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::StageX);
	dae::SceneManager::GetInstance().CreateScene(SceneNames::StageScore);
}
