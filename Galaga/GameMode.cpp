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

	galaga->m_StateMachine.EnterState<WaveOneState>();	
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
