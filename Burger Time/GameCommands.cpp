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
	auto game = GameManager::GetInstance().GetActiveGame();
	auto burgerTime = dynamic_cast<BurgerTimeGame*>(game);

	burgerTime->SkipLevel();
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

void PepperCommand::Execute()
{
	auto player = GetGameObject()->GetComponent<PlayerComponent>();
	player->Pepper();
};
