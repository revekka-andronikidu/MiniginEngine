#include "GameCommands.h"
#include <ServiceLocator.h>
#include <TimeManager.h>
#include <algorithm>
#include "PlayerComponent.h"
#include <GameManager.h>
#include "BurgerTimeGame.h"
#include "GameState.h"
#include "KeypadComponent.h"

using namespace dae;

void MuteGameCommand::Execute()
{
	ServiceLocator::GetAudioService().Mute();
}

void SkipLevelCommand::Execute()
{
	//Check only if in the GameMode
	//Is there next stage availible?

	auto game = GameManager::GetInstance().GetActiveGame();
	auto burgerTime = dynamic_cast<BurgerTimeGame*>(game);

	auto state = burgerTime->m_GameModeMachine.GetCurrentState();

	//make it work for all modes
	auto mode = dynamic_cast<InGameState*>(state);
	mode->NextStage();


}

void MenuMoveCommand::Execute()
{
	GetGameObject()->GetComponent<MenuComponent>()->ChangeMenuItem(m_direction);
	ServiceLocator::GetAudioService().PlayEffect(SoundID::SystemSound.id, 0.5f, false);
};


void MenuEnterCommand::Execute()
{
	GetGameObject()->GetComponent<MenuComponent>()->EnterMenuItem();
};


void KeypadMoveCommand::Execute()
{
	GetGameObject()->GetComponent<KeypadComponent>()->HandleMove(m_direction);
	ServiceLocator::GetAudioService().PlayEffect(SoundID::SystemSound.id, 0.5f, false);
};

void KeypadSelectCommand::Execute()
{
	GetGameObject()->GetComponent<KeypadComponent>()->Select();
	ServiceLocator::GetAudioService().PlayEffect(SoundID::SystemSound.id, 0.5f, false);
};

void BackToMenuCommand::Execute()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto burgerTime = dynamic_cast<BurgerTimeGame*>(game);

	burgerTime->m_GameModeMachine.EnterState<MainMenuState>();
}

void MoveCommand::Execute()
{
	auto player = GetGameObject()->GetComponent<PlayerComponent>();
	player->Move(m_Direction);
};

