#include "GameState.h"
#include <SceneManager.h>
#include <ServiceLocator.h>
#include "SceneFactory.h"
#include "GameManager.h"
#include "BurgerTimeGame.h"

using namespace dae;

void MainMenuState::OnEnter()
{
	//auto game = GameManager::GetInstance().GetActiveGame();
	//auto galaga = dynamic_cast<GalagaGame*>(game);
	auto& sceneManager = dae::SceneManager::GetInstance();


	if (!m_menuCreated)
	{
//		auto& scene = sceneManager.GetScene(SceneNames::MainMenu);
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

void SinglePlayerMode::OnEnter()
{
	auto& scene = SceneManager::GetInstance().CreateScene(SceneNames::Stage1);


	auto game = GameManager::GetInstance().GetActiveGame();
	auto burgerTime = dynamic_cast<BurgerTimeGame*>(game);

	burgerTime->m_CurrentStage += 1;
	burgerTime->LoadLevel();


	SceneManager::GetInstance().SetActiveScene(SceneNames::Stage1);
}